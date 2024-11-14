#include "dpi_symb_handler.hpp"

int main() {
    Dpi_symb_handler app_engine("./libwrdpengine.so","./libwrdpbundle.so");
    app_engine.load_symbols();
    app_engine.create_engine();
    app_engine.print_version();
    app_engine.activate_full_signatures();
    app_engine.get_list_app();

}
