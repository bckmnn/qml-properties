import QtQuick 2.12

ListView{
    id: root
    delegate: Component{
        Loader{
            source: modelData.delegate
        }
    }
}
