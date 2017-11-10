#ifdef TLIST_H
template <typename T> TList<T>::TNode::TNode()
{
    item = std::shared_ptr<T>();
    next = nullptr;
}

template <typename T> TList<T>::TNode::TNode(const std::shared_ptr<T>& obj)
{
    item = obj;
    next = nullptr;
}

template <typename T> TAllocator TList<T>::TNode::nodeAllocator(sizeof(TList<T>::TNode), 100);

template <typename T> void* TList<T>::TNode::operator new(size_t size)
{
    return nodeAllocator.allocate();
}

template <typename T> void TList<T>::TNode::operator delete(void* ptr)
{
    nodeAllocator.deallocate(ptr);
}

template <typename T> TList<T>::TList()
{
    head = std::make_shared<TNode>();
    length = 0;
}

template <typename T> bool TList<T>::IsEmpty() const
{
    return this->length == 0;
}

template <typename T> auto& TList<T>::getHead()
{
    return this->head->next;
}

template <typename T> auto&& TList<T>::getTail()
{
    auto tail = head->next;
    while (tail->next != nullptr) {
        tail = tail->next;
    }

    return tail;
}

template <typename T> int TList<T>::GetLength() const
{
    return this->length;
}

template <typename T> bool TList<T>::PushFront(const std::shared_ptr<T>& obj)
{
    auto Nitem = std::make_shared<TNode>(obj);
    std::swap(Nitem->next, head->next);
    std::swap(head->next, Nitem);
    length++;

    return true;
}

template <typename T> bool TList<T>::Push(const std::shared_ptr<T>& obj, int pos)
{
    if (pos == 1 || length == 0)
        return PushFront(obj);
    if (pos < 0 || pos > length + 1)
        return false;

    auto iter = head->next;
    int i = 0;

    while (i < pos - 2) {
        iter = iter->next;
        i++;
    }

    auto Nitem = std::make_shared<TNode>(obj);
    std::swap(Nitem->next, iter->next);
    std::swap(iter->next, Nitem);
    length++;

    return true;
}

template <typename T> bool TList<T>::PopFront()
{
    if (IsEmpty())
        return false;

    head->next = std::move(head->next->next);

    length--;

    return true;
}

template <typename T> bool TList<T>::Pop(int pos)
{
    if (pos < 1 || pos > length || IsEmpty())
        return false;
    if (pos == 1)
        return PopFront();

    auto iter = head->next;
    int i = 0;

    while (i < pos - 2) {
        iter = iter->next;
        i++;
    }

    iter->next = std::move(iter->next->next);
    length--;

    return true;
}

template <typename T> auto TList<T>::TNode::GetNext() const
{
    return this->next;
}

template <typename T> auto TList<T>::TNode::GetItem() const
{
    return this->item;
}

template <typename A> std::ostream& operator<< (std::ostream& os, const TList<A>& list)
{
    if (list.IsEmpty()) {
        os << "The list is empty!" << std::endl;
        return os;
    }

    auto tmp = list.head->GetNext();
    while(tmp != nullptr) {
        tmp->GetItem()->Print();
        tmp = tmp->GetNext();
    }

    return os;
}

template <typename T> auto TList<T>::psort(std::shared_ptr<TNode>& head)
{
    if (head == nullptr || head->next == nullptr) {
        return head;
    }

    auto partitionedEl = partition(head);
    auto leftPartition = partitionedEl->next;
    auto rightPartition = head;

    partitionedEl->next = nullptr;

    if (leftPartition == nullptr) {
        leftPartition = head;
        rightPartition = head->next;
        head->next = nullptr;
    }

    rightPartition = psort(rightPartition);
    leftPartition = psort(leftPartition);
    auto iter = leftPartition;
    while (iter->next != nullptr) {
        iter = iter->next;
    }

    iter->next = rightPartition;

    return leftPartition;
}

template <typename T> auto TList<T>::partition(std::shared_ptr<TNode>& head)
{
    if (head->next->next == nullptr) {
        if (head->next->GetItem()->getSquare() > head->GetItem()->getSquare()) {
            return head->next;
        } else {
            return head;
        }
    } else {
        auto i = head->next;
        auto pivot = head;
        auto lastElSwapped = (pivot->next->GetItem()->getSquare()
                          >= pivot->GetItem()->getSquare()) ? pivot->next : pivot;

        while ((i != nullptr) && (i->next != nullptr)) {
            if (i->next->GetItem()->getSquare() >= pivot->GetItem()->getSquare()) {
                if (i->next == lastElSwapped->next) {
                    lastElSwapped = lastElSwapped->next;
                } else {
                    auto tmp = lastElSwapped->next;
                    lastElSwapped->next = i->next;
                    i->next = i->next->next;
                    lastElSwapped = lastElSwapped->next;
                    lastElSwapped->next = tmp;
                }
            }

            i = i->next;
        }

        return lastElSwapped;
    }

}

template <typename T> void TList<T>::sort()
{
    head->next = psort(head->next);
}

template <typename T> void TList<T>::parSort()
{
    head->next = pparsort(head->next);
}

template <typename T> auto TList<T>::pparsort(std::shared_ptr<TNode>& head)
{
    if (head == nullptr || head->next == nullptr) {
        return head;
    }

    auto partitionedEl = partition(head);
    auto leftPartition = partitionedEl->next;
    auto rightPartition = head;

    partitionedEl->next = nullptr;

    if (leftPartition == nullptr) {
        leftPartition = head;
        rightPartition = head->next;
        head->next = nullptr;
    }

    std::packaged_task<std::shared_ptr<TNode>(std::shared_ptr<TNode>&)>
        task1(std::bind(&TList<T>::pparsort, this, std::placeholders::_1));
    std::packaged_task<std::shared_ptr<TNode>(std::shared_ptr<TNode>&)>
        task2(std::bind(&TList<T>::pparsort, this, std::placeholders::_1));
    auto rightPartitionHandle = task1.get_future(); 
    auto leftPartitionHandle = task2.get_future();


    std::thread(std::move(task1), std::ref(rightPartition)).join(); 
    rightPartition = rightPartitionHandle.get();
    std::thread(std::move(task2), std::ref(leftPartition)).join();
    leftPartition = leftPartitionHandle.get();
    auto iter = leftPartition;
    while (iter->next != nullptr) {
        iter = iter->next;
    }

    iter->next = rightPartition;

    return leftPartition;
}


#endif