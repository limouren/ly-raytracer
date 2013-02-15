cpplint --filter=-build/include,-legal/copyright,-build/header_guard,-build/namespaces src/* config/* 2>&1 main.cpp | grep -v 'Done processing'
