#ifndef OBJECT_HPP_INCLUDED
#define OBJECT_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

enum ObjectType
{
    object_Invalid,
    object_First = 1,
    object_Food = object_First,
    object_Water,
    object_Weapon,
    object_Armor,
    object_Ammo,

    object_Count
};

struct ObjectData;

typedef uint32_t ObjectId;

class Object;
typedef std::shared_ptr<Object> ObjectPtr;

class Object final
{
private:
    ObjectType type_;
    ObjectId uid_;
    uint32_t weight_;
    std::string name_;
    uint32_t sprite_;

    std::unordered_map<std::string, uint32_t> properties_;
    static std::unordered_map<ObjectId, ObjectPtr> objects_;
    Object();

public:
    class Instance
    {
        ObjectId uid_;
        uint32_t quantity_;
        ObjectPtr parent_;
    public:
        Instance(ObjectPtr obj, uint32_t qty = 1);

        ObjectId GetId() const;
        uint32_t GetQuantity() const;

        ObjectPtr GetParent() const;
        ObjectPtr GetParent();

        void SetQuantity(uint32_t qty);
        void ChangeQuantity(int32_t qty);
        const std::string& GetName() const;
    };

    static ObjectPtr GetObject(ObjectId id);
    static ObjectPtr BuildFromString(const std::string& str);
    std::string ToString() const;
    static Instance CreateInstance(ObjectId id, uint32_t qty);
    static ObjectType StringToType(const std::string& str);

    ObjectId GetId() const;
    ObjectType GetType() const;
    const std::string& GetName() const;
    uint32_t GetWeight() const;
    uint32_t GetSprite() const;
    uint32_t GetProperty(const std::string& name) const;
    void SetProperty(const std::string& name, const uint32_t& value);
};

#endif
