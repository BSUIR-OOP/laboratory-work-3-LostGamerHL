#include "serialization.h"

using std::string;

class BaseClass : public iSerializable
{
public:
    virtual ~BaseClass () {}

    virtual void serialize (Node node) override
    {
        node.set_name ("BaseClass");
        node.serialize (m_int, "int");
        node.serialize (m_uint, "uint");
    }

private:
    int m_int = 10;
    unsigned int m_uint = 200;
};


class BruhClass : public BaseClass
{
public:
    virtual ~BruhClass () {}

    virtual void serialize (Node node) override
    {
        BaseClass::serialize (node);
        node.set_name ("BruhClass");
        node.serialize (m_float, "float");
        node.serialize (m_double, "double");
    }

private:
    float m_float = 1.0f;
    double m_double = 2.0;
};


class CockAndBallTorture : public BaseClass
{
public:
    virtual ~CockAndBallTorture () {}

    virtual void serialize (Node node) override
    {
        BaseClass::serialize (node);
        node.set_name ("CockAndBallTorture");
        node.serialize (m_str, "string");
        node.serialize (m_bool, "boolean");
    }

private:
    string m_str;
    bool m_bool = true;
};


class Container
{
public:
    ~Container ()
    {
        for (BaseClass* ptr : vec_ptr) delete ptr;
        vec_ptr.clear ();
        vec_arg.clear ();
    }

    void serialize (Node node)
    {
        node.set_name ("SomeContainers");
        node.serialize (vec_ptr, "containerPtr", "myclass_");
        node.serialize (vec_arg, "containerArg", "myclass_");
    }

private:
    std::vector<BaseClass*> vec_ptr;
    std::vector<BaseClass> vec_arg;
};

void init_prototypes (Prototypes::PrototypeFactory* factory)
{
    printf("init_prototypes\n");
    factory->set_object<BaseClass> ("BaseClass");
    factory->set_object<BruhClass> ("BruhClass");
    factory->set_object<CockAndBallTorture> ("CockAndBallTorture");
}

int main (int argc, char* argv[])
{
    Serializer* document = new Serializer ();
    Container* container = new Container ();

    document->deserialize (container, "doc");
    document->serialize (container, "document");

    delete container;
    delete document;

    return 0;
}
