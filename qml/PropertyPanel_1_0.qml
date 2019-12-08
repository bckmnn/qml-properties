import QtQuick 2.12

ListView{
    id: root
    delegate: Component{
        Loader{
            width: root.width
            source: modelData.delegate
        }
    }
}
