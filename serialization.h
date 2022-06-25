#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include "pugiconfig.hpp"
#include "pugixml.hpp"
#include "patterns.h"

namespace Database
{
using namespace pugi;
using std::string;


class Node;

class iSerializable
{
public:
    virtual void serialize (Node node) = 0;
};

//typedef std::shared_ptr<iSerializable> pSerializable;

template <typename Data> struct serializable
{
public:
    Data& value;

    serializable (Data& _value) : value (_value) {}
};

template<typename T>
struct identity { typedef T type; };

class Node
{
public:
    Node (xml_node node) : m_node (node) {}

    void set_name (const string& name)
    {
        m_node.set_name (name.c_str ());
    }

    template <typename T> void serialize (T& value, const string& name)
    {
        value.serialize (get_node (name));
    }

    template <> void serialize (int& value, const string& name)
    {
        xml_attribute it = m_node.attribute (name.c_str ());
        if (it) value = it.as_int ();
        else m_node.append_attribute (name.c_str ()).set_value (value);
    }

    template <> void serialize (unsigned int& value, const string& name)
    {
        xml_attribute it = m_node.attribute (name.c_str ());
        if (it) value = it.as_uint ();
        else m_node.append_attribute (name.c_str ()).set_value (value);
    }

    template <> void serialize (double& value, const string& name)
    {
        xml_attribute it = m_node.attribute (name.c_str ());
        if (it) value = it.as_double ();
        else m_node.append_attribute (name.c_str ()).set_value (value);
    }

    template <> void serialize (float& value, const string& name)
    {
        xml_attribute it = m_node.attribute (name.c_str ());
        if (it) value = it.as_double ();
        else m_node.append_attribute (name.c_str ()).set_value (value);
    }

    template <> void serialize (bool& value, const string& name)
    {
        xml_attribute it = m_node.attribute (name.c_str ());
        if (it) value = it.as_bool ();
        else m_node.append_attribute (name.c_str ()).set_value (value);
    }

    template <> void serialize (string& value, const string& name)
    {
        xml_attribute it = m_node.attribute (name.c_str ());
        if (it) value = it.as_string ();
        else m_node.append_attribute (name.c_str ()).set_value (value.c_str ());
    }

    template <> void serialize (iSerializable*& object, const string& name)
    {
        if (!object) m_factory->get_object (object, m_node.find_child_by_attribute ("name", name.c_str ()).name ());
        //else m_node.set_name (object->get_name ().c_str ());
        object->serialize (get_node_by_attr (name));
    }

    template <template <typename T, class alloc> class Container, typename Data, typename alloc> void serialize (
            Container<Data*, alloc>& container,
            const string& name,
            const string& subname
        )
    {
        Node node = get_node (name);
        size_t size (container.size ());
        node.serialize (size, "size");
        if (container.empty ()) container.assign (size, nullptr);

        size_t count (0);

        for (auto i = container.begin (); i < container.end (); ++i, ++count)
            node.serialize ((iSerializable*&) *i, subname + std::to_string (count));
    }

    template <template <typename T, class alloc> class Container, typename Data, typename alloc> void serialize (
            Container<Data, alloc>& container,
            const string& name,
            const string& subname
        )
    {
        Node node = get_node (name);
        unsigned int size (container.size ());
        node.serialize (size, "size");
        if (container.empty ()) container.assign (size, Data ());

        size_t count (0);

        for (auto i = container.begin (); i < container.end (); ++i)
            i->serialize (node.get_node_by_attr (subname + std::to_string (count++)));
    }

private:
    xml_node m_node;
    PrototypeFactory m_factory;

    Node get_node (const string& name)
    {
        xml_node ch = m_node.child (name.c_str ());
        if (!ch) ch = m_node.append_child (name.c_str ());
        return ch;
    }

    Node get_node_by_attr (const string& name)
    {
        xml_node ch = m_node.find_child_by_attribute ("name", name.c_str ());
        if (!ch)
        {
            ch = m_node.append_child ("");
            ch.append_attribute ("name").set_value (name.c_str ());
        }
        return ch;
    }
};


class Serializer
{
public:
    Serializer() {}

    template <class Serializable> void serialize (Serializable* object, const string& filename)
    {
        object->serialize (m_document.append_child (""));
        m_document.save_file ((filename+".xml").c_str ());
        m_document.reset ();
    }

    template <class Serializable> void deserialize (Serializable* object, const string& filename)
    {
        if (m_document.load_file ((filename + ".xml").c_str ())) object->serialize (m_document.first_child ());
        m_document.reset ();
    }

private:
    xml_document m_document;
    PrototypeFactory m_factory;
};


}

using Database::iSerializable;
using Database::Node;
using Database::Serializer;
using Database::serializable;

#endif
