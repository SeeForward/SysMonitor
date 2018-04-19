
#ifndef __SINGLETON_H__
#define __SINGLETON_H__

template <typename T>
class Singleton
{
public:
    typedef T object_type;

    static T& Instance()
    {
        static T obj;
        return obj;
    }

private:
    struct object_creator
    {
        object_creator()
        {
            Singleton<T>::Instance();
        }
    };

    static object_creator create_object;
};

template <typename T>
typename Singleton<T>::object_creator Singleton<T>::create_object;

#endif //__SINGLETON_H__
