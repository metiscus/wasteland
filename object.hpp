#ifndef OBJECT_HPP_INCLUDED
#define OBJECT_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <string>

enum ObjectType
{
    object_Invalid,
    object_Food,
    object_Water,
    object_Weapon,
    object_Armor.
    
    object_Count
};

struct ObjectData;

class Object
{
private:
    std::unique_ptr<ObjectData> data_;
    
public:
    Object();
    static std::shared_ptr<Object> BuildFromString(const std::string& str);
    std::string ToString() const;
};

#endif
