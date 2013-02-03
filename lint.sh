cpplint --filter=-build/include,-legal/copyright,-build/header_guard,-build/namespaces src/* config/* 2>&1 | grep -v 'Done processing'
