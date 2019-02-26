package memory;

public class CycledList<T> {
    private class Item {
        private T data;

        private Item next = this;

        Item(T data) {
            this.data = data;
        }

        Item(T data, Item next) {
            this.data = data;
            this.next = next.next;
            next.next = this;
        }
    }

    private Item rootItem = null;
    private int num = 0;

    public void add(T data) {
        if (rootItem == null) rootItem = new Item(data);
        else new Item(data, rootItem);
        num++;
    }

    public T get() {
        return rootItem.data;
    }

    public T getNext() {
        rootItem = rootItem.next;
        return rootItem.data;
    }

    public void changeData(T newData) {
        rootItem.data = newData;
        getNext();
    }

    public int getNumber() {
        return num;
    }
}
