//
// Created by roma on 14.11.24.
//

#pragma once
#include <dlfcn.h>

#include <filesystem>
#include <iostream>

typedef int (*dpi_bundle_signature_callback_t)(struct dpi_bundle* bundle, struct dpi_signature* sig, void* arg);

// todo: сделать свои исключения
class Dpi_symb_handler {
   private:
    std::filesystem::path m_lib_bundle;
    std::string m_config_engin{"injection_mode=stream;nb_workers=1;nb_flows=0;log_level=non"}; // конфигурация движка 32
    struct dpi_engine* m_engine = nullptr;                                                     // движок 8
    struct dpi_bundle* m_bundle = nullptr;                                                     // пакет 8
    std::unique_ptr<void, decltype(&dlclose)> m_library_engine;                                // движок библиотека 8
    void (*m_se_engine_destroy)(dpi_engine*) = nullptr;                                        // движок разрушить 8
    struct dpi_engine* (*m_se_engine_create)(char const*) = nullptr;                           // движок создать 8
    struct dpi_bundle* (*m_sb_create)(struct dpi_engine*, char const*) = nullptr;              // пакет создать 8
    char const* (*m_se_get_version)() = nullptr;                                               // движок версия 8
    char const* (*m_sb_get_version)(struct dpi_bundle*) = nullptr;                             // пакет версия 8
    int (*m_sb_signature_enable_all)(struct dpi_bundle*) = nullptr;                            // пакет включить все сигнатуры 8
    int (*m_sb_get_signature)(struct dpi_bundle*, dpi_bundle_signature_callback_t callback, void*) = nullptr; // пакет получить включенные сигнатуры
    static int (*m_pr_get_appid)(struct dpi_bundle*, struct dpi_signature*);                                  // appid
    static char const* (*m_pr_get_application)(struct dpi_bundle*, struct dpi_signature*); // application
    static char const* (*m_pr_get_category)(struct dpi_bundle*, struct dpi_signature*);    // category
    static char const* (*m_pr_get_description)(struct dpi_bundle*, struct dpi_signature*); // description

   public:
    Dpi_symb_handler(std::filesystem::path&& path_to_engine, std::filesystem::path&& path_to_bundle);
    ~Dpi_symb_handler();
    Dpi_symb_handler(Dpi_symb_handler&&) = default;
    Dpi_symb_handler& operator=(Dpi_symb_handler&&) = default;
    Dpi_symb_handler(Dpi_symb_handler const&) = delete;
    Dpi_symb_handler& operator=(Dpi_symb_handler const&) = delete;
    void load_symbols();
    void create_engine();
    void print_version();
    void get_list_app();
    void activate_full_signatures();
    static int get_app(struct dpi_bundle* bundle, struct dpi_signature* sig, void* arg);

   private:
    void load_symbols_print();
    void load_symbols_bundle();
    void load_symbols_engine();
};
