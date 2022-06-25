#ifndef PATTERNS_H
#define PATTERNS_H

#include <vector>
#include <list>
#include <string>
#include <memory>
#include <algorithm>
#include <exception>
#include <stdexcept>

template <typename T> class Singleton
{
public:
    Singleton ()
    {
        ++count;
        if (!instance) instance = new T;
    }

    Singleton (const Singleton&)
    {
        ++count;
    }

    ~Singleton ()
    {
        if (!--count) delete instance;
    }

    T* operator-> ()
    {
        return instance;
    }

private:
    static T* instance;
    static size_t count;

    const Singleton& operator= (const Singleton&);
};

template <typename T> T* Singleton<T>::instance (nullptr);
template <typename T> size_t Singleton<T>::count (0);



namespace Prototypes { class PrototypeFactory; }

void init_prototypes (Prototypes::PrototypeFactory*);



namespace Prototypes
{


    using std::string;

    class PrototypeFactory
    {
    public:
        template <typename T> void set_object (const string& name)
        {
            if (std::find_if (
                m_prototypes.begin (),
                m_prototypes.end (),
                [name] (pObject obj) { return obj->name == name; }
                ) == m_prototypes.end ()
            )
                m_prototypes.push_back (std::make_shared<Object<T>> (name));
        }

        template <typename T> void get_object (T*& object, const string& name) const
        {
            auto it = find_if (m_prototypes.begin (), m_prototypes.end (), [name] (pObject obj) { return obj->name == name; });
            if (it != m_prototypes.end ()) object = (T*) (*it)->get_copy ();
            else throw std::runtime_error ("Prototype wasn't found!");
        }

    protected:
        friend class Singleton<PrototypeFactory>;
        PrototypeFactory () { init_prototypes (this);}

        PrototypeFactory (const PrototypeFactory&);
        PrototypeFactory& operator= (const PrototypeFactory&);

    private:
        struct ObjectBase
        {
            string name;

            ObjectBase (const string& _name) : name (_name) {}
            virtual ~ObjectBase () {}

            virtual void* get_copy () const = 0;
        };

        template <typename Data> struct Object : public ObjectBase
        {
            Object (const string& _name) : ObjectBase (_name) {}

            virtual void* get_copy () const override
            {
                return (void*) new Data ();
            }
        };

        typedef std::shared_ptr<ObjectBase> pObject;

        std::vector<pObject> m_prototypes;
    };
}

typedef Singleton<Prototypes::PrototypeFactory> PrototypeFactory;
//#define OBJECT_NAME (x) \
//    private:                                                                        \
//        virtual ::std::string get_name () const override { return object_name; }     \
//        static ::std::string const object_name = x;

#endif
