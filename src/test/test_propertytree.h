#pragma once

#include <string>
#include <set>
#include <exception>
#include <iostream>
#include <deque>
#include <any>

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace test_propertytree
{
    //<debug>
    //    <filename>debug.log</filename>
    //    <modules>
    //          <module>Finance</module>
    //          <module>Admin</module>
    //          <module>HR</module>
    //    </modules>
    //    <level>2</level>
    //</debug>

    struct debug_settings
    {
        std::string m_file;                  //log filename
        int m_level;                         //debug level
        std::set<std::string> m_modules;     //modules where logging is enabled

        void load(std::string const& filename)
        {
            //Create empty property tree object
            boost::property_tree::ptree tree;

            //Parse the XML into the property tree.
            boost::property_tree::read_xml(filename, tree);

            //Use the throwing version of get to find the debug filename.
            //If the path cannot be resolved, an exception is thrown.
            m_file = tree.get<std::string>("debug.filename");

            //Use the default-value version of get to find the debug level.
            //Note that the default value is used to deduce the target type.(默认值用于推导目标类型)
            m_level = tree.get("debug.level", 0);

            //Use get_child to find the node containing the modules, and iterate over its
            //children. If the path cannot be resolved, get_child throws.
            //A C++11 for-range loop would also work.
            //BOOST_FOREACH(boost::property_tree::ptree::value_type & v, tree.get_child("debug.modules")) {
            //    //The data function is used to access the data stored in a node.
            //    m_modules.insert(v.second.data());
            //}
            for (boost::property_tree::ptree::value_type& v : tree.get_child("debug.modules"))
            {
                m_modules.insert(v.second.data());
            }
        }

        void save(std::string const& filename)
        {
            //Create an empty property tree object.
            boost::property_tree::ptree tree;

            //Put the simple values into the tree.The integer is automatically converted to a string.
            //Note that the "debug" node is automatically created if it doesn't exist.
            tree.put("debug.filename", m_file);
            tree.put("debug.level", m_level);

            //Add all the modules. Unlike put, which overwrites existing nodes, adds a new node at the 
            //lowest level, so the "modules" node will have multiple "module" children.
            //BOOST_FOREACH(const std::string & name, m_modules)
            //    tree.add("debug.modules.module", name);
            for (const std::string& name : m_modules)
            {
                tree.add("debug.modules.module", name);
            }

            //Write property tree to XML file
            boost::property_tree::write_xml(filename, tree);
        }
    };

}

namespace test_property_type
{
    struct value_item
    {
        using group_type = std::deque<value_item>;    //当value类型为group_type时，type固定为std::deque<value_item>

        boost::property_tree::ptree to_ptree() const
        {
            boost::property_tree::ptree value_item_tree;
            value_item_tree.put("name", name);
            value_item_tree.put("type", type);
            if (value.type() == typeid(group_type))
            {
                boost::property_tree::ptree sub_value_item_tree;
                group_type value_ = std::any_cast<group_type>(value);
                std::for_each(value_.begin(), value_.end(), [&](value_item const& item) {
                    sub_value_item_tree.add_child("item", item.to_ptree());
                    });

                value_item_tree.put_child("value", sub_value_item_tree);
            }
            else
            {
                value_item_tree.put("value", std::any_cast<int>(value));   //?
            }
            return value_item_tree;
        }

        std::string name;
        std::string type;  //指定value的类型
        std::any value;
    };

    struct root
    {
        std::deque<value_item> kiwi_fast;

        boost::property_tree::ptree to_ptree() const
        {
            boost::property_tree::ptree kiwi_fast_tree;

            std::for_each(kiwi_fast.begin(), kiwi_fast.end(), [&](value_item const& item) {
                kiwi_fast_tree.add_child("item", item.to_ptree());
            });

            boost::property_tree::ptree root_tree;
            root_tree.put_child("kiwi_fast", kiwi_fast_tree);

            return root_tree;
        }
    };
}