#ifdef TLIST_H
template <typename T> template <typename N, typename M> TList<T>::TIterator<N, M>::TIterator(const N& node)
{
    nodePtr = node;
}

template <typename T> template <typename N, typename M> std::shared_ptr<M> TList<T>::TIterator<N, M>::operator* ()
{
    return nodePtr->GetItem();
}

template <typename T> template <typename N, typename M> std::shared_ptr<M> TList<T>::TIterator<N, M>::operator-> ()
{
    return nodePtr->GetItem();
}

template <typename T> template <typename N, typename M> void TList<T>::TIterator<N, M>::operator ++ ()
{
    nodePtr = nodePtr->GetNext();
}

template <typename T> template <typename N, typename M> bool TList<T>::TIterator<N, M>::operator == (TIterator const& other)
{
    return nodePtr == other.nodePtr;
}

template <typename T> template <typename N, typename M> bool TList<T>::TIterator<N, M>::operator != (TIterator const& other)
{
    return !(*this == other);
}

#endif