cpplint --filter=-build/include,-legal/copyright,-build/header_guard src/* config/* 2>&1 | grep -v 'Done processing'
