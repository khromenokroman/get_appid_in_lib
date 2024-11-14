//
// Created by roma on 14.11.24.
//

#include "dpi_symb_handler.hpp"

int (*Dpi_symb_handler::m_pr_get_appid)(struct dpi_bundle*, struct dpi_signature*) = nullptr;
char const* (*Dpi_symb_handler::m_pr_get_application)(struct dpi_bundle*, struct dpi_signature*) = nullptr;
char const* (*Dpi_symb_handler::m_pr_get_category)(struct dpi_bundle*, struct dpi_signature*) = nullptr;
char const* (*Dpi_symb_handler::m_pr_get_description)(struct dpi_bundle*, struct dpi_signature*) = nullptr;

Dpi_symb_handler::Dpi_symb_handler(std::filesystem::path&& path_to_engine, std::filesystem::path&& path_to_bundle)
    : m_library_engine{dlopen(path_to_engine.c_str(), RTLD_NOW | RTLD_GLOBAL), &dlclose},
      m_lib_bundle{std::move(path_to_bundle)} {
    if (!m_library_engine) {
        throw std::runtime_error(dlerror());
    }
}
Dpi_symb_handler::~Dpi_symb_handler() { m_se_engine_destroy(m_engine); }

void Dpi_symb_handler::load_symbols() {
    load_symbols_engine();
    load_symbols_bundle();
    load_symbols_print();
}

void Dpi_symb_handler::load_symbols_engine() {
    m_se_get_version = reinterpret_cast<char const* (*)()>(dlsym(m_library_engine.get(), "dpi_engine_get_version"));
    if (m_se_get_version == nullptr) {
        throw std::runtime_error(std::string("Failed to load symbol dpi_engine_get_version: ") + std::string(dlerror()));
    }

    m_se_engine_create = reinterpret_cast<struct dpi_engine* (*)(char const*)>(dlsym(m_library_engine.get(), "dpi_engine_create"));
    if (m_se_engine_create == nullptr) {
        throw std::runtime_error(std::string("Failed to load symbol dpi_engine_create: ") + std::string(dlerror()));
    }

    m_se_engine_destroy = reinterpret_cast<void (*)(dpi_engine*)>(dlsym(m_library_engine.get(), "dpi_engine_destroy"));
    if (m_se_engine_destroy == nullptr) {
        throw std::runtime_error(std::string("Failed to load symbol dpi_engine_destroy: ") + std::string(dlerror()));
    }
    std::cout << "Load symbols engine is done" << std::endl;
}

void Dpi_symb_handler::load_symbols_bundle() {
    m_sb_create = reinterpret_cast<struct dpi_bundle* (*)(struct dpi_engine*, char const*)>(dlsym(m_library_engine.get(), "dpi_bundle_create_from_file"));
    if (m_sb_create == nullptr) {
        throw std::runtime_error(std::string("Failed to load symbol dpi_bundle_create_from_file: ") + std::string(dlerror()));
    }
    m_sb_get_version = reinterpret_cast<char const* (*)(struct dpi_bundle*)>(dlsym(m_library_engine.get(), "dpi_bundle_get_version"));
    if (m_sb_get_version == nullptr) {
        throw std::runtime_error(std::string("Failed to load symbol dpi_bundle_get_version: ") + std::string(dlerror()));
    }
    m_sb_signature_enable_all = reinterpret_cast<int (*)(struct dpi_bundle*)>(dlsym(m_library_engine.get(), "dpi_bundle_signature_enable_all"));
    if (m_sb_signature_enable_all == nullptr) {
        throw std::runtime_error(std::string("Failed to load symbol dpi_bundle_signature_enable_all: ") + std::string(dlerror()));
    }
    m_sb_get_signature = reinterpret_cast<int (*)(struct dpi_bundle*, dpi_bundle_signature_callback_t callback, void*)>(
        dlsym(m_library_engine.get(), "dpi_bundle_signature_foreach_enabled"));
    if (m_sb_get_signature == nullptr) {
        throw std::runtime_error(std::string("Failed to load symbol dpi_bundle_signature_foreach_enabled: ") + std::string(dlerror()));
    }
    std::cout << "Load symbols bundle is done" << std::endl;
}

void Dpi_symb_handler::load_symbols_print() {
    m_pr_get_appid = reinterpret_cast<int (*)(struct dpi_bundle*, struct dpi_signature*)>(dlsym(m_library_engine.get(), "dpi_signature_id_get"));
    if (m_pr_get_appid == nullptr) {
        throw std::runtime_error(std::string("Failed to load symbol dpi_signature_id_get: ") + std::string(dlerror()));
    }
    m_pr_get_application =
        reinterpret_cast<char const* (*)(struct dpi_bundle*, struct dpi_signature*)>(dlsym(m_library_engine.get(), "dpi_signature_longname_get"));
    if (m_pr_get_application == nullptr) {
        throw std::runtime_error(std::string("Failed to load symbol dpi_signature_longname_get: ") + std::string(dlerror()));
    }
    m_pr_get_category = reinterpret_cast<char const* (*)(struct dpi_bundle*, struct dpi_signature*)>(dlsym(m_library_engine.get(), "dpi_signature_family_get"));
    if (m_pr_get_category == nullptr) {
        throw std::runtime_error(std::string("Failed to load symbol dpi_signature_family_get: ") + std::string(dlerror()));
    }
    m_pr_get_description =
        reinterpret_cast<char const* (*)(struct dpi_bundle*, struct dpi_signature*)>(dlsym(m_library_engine.get(), "dpi_signature_description_get"));
    if (m_pr_get_description == nullptr) {
        throw std::runtime_error(std::string("Failed to load symbol dpi_signature_tags_get: ") + std::string(dlerror()));
    }
    std::cout << "Load symbols for view is done" << std::endl;
}
void Dpi_symb_handler::create_engine() {
    m_engine = m_se_engine_create(m_config_engin.c_str());
    if (!m_engine) {
        throw std::runtime_error("Failed to create engine");
    } else {
        std::cout << "Create engine is done" << std::endl;
    }

    m_bundle = m_sb_create(m_engine, m_lib_bundle.c_str());
    if (!m_bundle) {
        throw std::runtime_error("Failed to add bundle");
    } else {
        std::cout << "Add bundle is done" << std::endl;
    }
}

int Dpi_symb_handler::get_app(struct dpi_bundle* bundle, struct dpi_signature* sig, void* arg) {
    int appid = Dpi_symb_handler::m_pr_get_appid(bundle, sig);
    const char* application = Dpi_symb_handler::m_pr_get_application(bundle, sig);
    const char* category = Dpi_symb_handler::m_pr_get_category(bundle, sig);
    const char* description = Dpi_symb_handler::m_pr_get_description(bundle, sig);
    std::cout << "CATEGORY: " << category << " APPLICATION: " << application << " APPID: " << appid << ", DESCRIPTION: " << description << '\n';
    return 0;
}
void Dpi_symb_handler::print_version() {
    std::cout << "Version engine: " << m_se_get_version() << std::endl;
    std::cout << "Version bundle: " << m_sb_get_version(m_bundle) << std::endl;
}
void Dpi_symb_handler::activate_full_signatures() {
    auto get_enable_signatures = m_sb_signature_enable_all(m_bundle);
    // todo: убрать магическое число
    if (get_enable_signatures == -4) {
        throw std::runtime_error("Failed enable signatures");
    }
    std::cout << "Enable signatures: " << get_enable_signatures << std::endl;
}
void Dpi_symb_handler::get_list_app() {
    auto res = m_sb_get_signature(m_bundle, get_app, nullptr);
    std::cout << "res: " << res << std::endl;
}
