#ifdef TLIST_H

template <typename Q, typename O> TStack<Q, O>::TStack()
{
    head = std::make_shared<Node>(Node());
    count = 0;
}

template <typename Q, typename O> TStack<Q, O>::Node::Node()
{
    next = nullptr;
    itemsInNode = 0;
}

template <typename Q, typename O> TStack<Q, O>::Node::Node(const O& item)
{
    data.PushFront(item);
    itemsInNode = 1;
}

template <typename Q, typename O> void TStack<Q, O>::removeByType(const int& type)
{
    auto tmp = head;
    while(tmp) {
        if (tmp->itemsInNode) {
            for (int i = 0; i < 5; ++i) {
                auto iter = tmp->data.begin();

                for (int k = 0; k < tmp->data.GetLength(); ++k) {
                    if (iter->type() == type) {
                        tmp->data.Pop(k + 1);
                        tmp->itemsInNode--;
                        break;
                    }
                    ++iter;
                }
            }
        }
        tmp = tmp->next;
    }
}

template <typename Q, typename O> void TStack<Q, O>::push(const O& item)
{
    auto tmp = head;
    if (tmp->itemsInNode < 5) {
        tmp->data.PushFront(item);
        tmp->itemsInNode++;
    } else {
        auto newNode = std::make_shared<Node>(Node(item));
        newNode->next = head;
        head = newNode;
        ++count;
    }
}


template <typename Q, typename O> void TStack<Q, O>::print()
{
    auto tmp = head;
    while (tmp) {
        if (tmp->itemsInNode) {
            tmp->data.sort();
            for (const auto &i: tmp->data) {
                i->Print();
            }
            std::cout << std::endl;
        }
        tmp = tmp->next;
    }
}


template <typename Q, typename O> void TStack<Q, O>::removeLesser(const double& sqr)
{
    auto tmp = head;
    while(tmp) {
        if (tmp->itemsInNode) {
            for (int i = 0; i < 5; ++i) {
                auto iter = tmp->data.begin();

                for (int k = 0; k < tmp->data.GetLength(); ++k) {
                    if (iter->getSquare() < sqr) {
                        tmp->data.Pop(k + 1);
                        tmp->itemsInNode--;
                        break;
                    }
                    ++iter;
                }
            }
        }
        tmp = tmp->next;
    }
}

#endif