Сборка
````bash
mkdir build && cd build
cmake ..
cmake --build .
````

Output:
````
Load symbols engine is done
Load symbols bundle is done
Load symbols for view is done
[msculib] log file </home/roma/CLionProjects/untitled28/cmake-build-debug/untitled28.logs/2024-11-14/untitled28.241114.223156.f-15.766935.log.txt> created
? [msculib] msculib_set_log_level: attempt to set new debug level "0", current is "3" (0-NONE, 1-ERROR, 2-WARNING, 3-INFORMATION, 4-VERBOSE, 5-DEBUG, 6-TRACE) - "success"
Create engine is done
Add bundle is done
Version engine: 0.1.39.2024-10-24
Version bundle: 0.1.410.2024-11-13
Enable signatures: 333
CATEGORY: Network Service APPLICATION: TCP APPID: 205, DESCRIPTION: Transmission Control Protocol. Base protocol for flow processing.
Registers 'tcp.pre' and 'tcp' heuristic lists.
Registers 'tcp.port' table.

CATEGORY: Network Service APPLICATION: UDP APPID: 216, DESCRIPTION: User Datagram Protocol. Base protocol for flow processing.
Registers 'udp.pre' and 'udp' heuristic lists.
Registers 'udp.port' table.

CATEGORY: Network Service APPLICATION: SCTP APPID: 180, DESCRIPTION: Stream Control Transmission Protocol. Base protocol for flow processing.
Registers 'sctp.pre' and 'sctp' heuristic lists.
Registers 'sctp.port' table.

CATEGORY: Encrypted APPLICATION: SSL (TLS) APPID: 199, DESCRIPTION: Secure Socket Layer / TLS packets dissector.
Registers itself in heuristic list 'tcp'.

CATEGORY: Network Service APPLICATION: HTTP APPID: 67, DESCRIPTION: HyperText Transfer Protocol packets dissector.
Registers itself in heuristic list 'tcp'.
Registers itself in heuristic list 'udp' - detection only.
Registers itself in heuristic list 'sctp' - detection only.

CATEGORY: Network Service APPLICATION: NTP APPID: 137, DESCRIPTION: Network Time Protocol packets dissector.
Registers itself in heuristic list 'udp'.

CATEGORY: Network Service APPLICATION: QUIC APPID: 2521, DESCRIPTION: Quic UDP Internet Connections packets dissector.
Registers itself in heuristic list 'udp'.
````

Приложение загружает библиотеки находит символы получает информацию

Использование:

````
Dpi_symb_handler app_engine("./libwrdpengine.so","./libwrdpbundle.so");
app_engine.load_symbols();
app_engine.create_engine();
app_engine.print_version();
app_engine.activate_full_signatures();
app_engine.get_list_app();
````