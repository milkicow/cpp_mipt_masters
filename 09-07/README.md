Влияние на производительность оригинального паттерна Pimpl:

```cpp
class Entity : private boost::noncopyable
{
public :
    Entity();
    Entity(Entity && other);

   ~Entity();

    auto & operator=(Entity && other);
    void test() const;

private :
	class Implementation;
    Implementation * m_pimpl = nullptr;
};
```

1. Аллокация нового объекта на куче на каждый объект
2. Освобождение объекта на куче при уничтожении каждого объекта
3. Косвенная адресация через указатель *m_pimpl* при каждом вызове метода
4. Почти всегда cache miss: *Entity* и *Implementation* расположены в разных местах памяти

В случае использования in-place Pimpl:

```cpp
alignas(std::max_align_t) std::array<std::byte, 16> m_buffer;
```
1. Аллокация на куче отсутствует — как и освобождение памяти
2. Косвенная адресация сохраняется — через get()
3. Cache miss маловероятен — *Entity* и *Implementation* расположены в едином непрерывном блоке памяти, данные с высокой вероятностью находятся в одной кэш-линии