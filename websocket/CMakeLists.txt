add_library(websocket STATIC websocket_client.cc)
target_link_libraries(websocket PUBLIC utils ssl crypto)
