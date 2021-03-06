#ifndef HIKARI_CORE_GAME_MAP_MAPLOADER
#define HIKARI_CORE_GAME_MAP_MAPLOADER

#include "hikari/core/Platform.hpp"
#include "hikari/core/geom/Rectangle2D.hpp"
#include "hikari/core/util/Service.hpp"
#include "hikari/core/util/TilesetCache.hpp"

#include <memory>
#include <json/value.h>
#include <string>

namespace hikari {

    class AnimationSetCache;
    class ImageCache;
    class TilesetCache;
    class Map;
    class Door;
    class Room;
    class RoomTransition;
    class Spawner;

    typedef std::shared_ptr<Map> MapPtr;
    typedef std::shared_ptr<Room> RoomPtr;
    typedef std::shared_ptr<Spawner> SpawnerPtr;

    class HIKARI_API MapLoader : public Service {
    private:
        static const char* PROPERTY_NAME_TILESET;
        static const char* PROPERTY_NAME_ROOMS;
        static const char* PROPERTY_NAME_GRIDSIZE;
        static const char* PROPERTY_NAME_MUSICID;
        static const char* PROPERTY_NAME_MUSICNAME;
        static const char* PROPERTY_NAME_SPECIAL_ROOMS;
        static const char* PROPERTY_NAME_SPECIAL_ROOM_STARTING;
        static const char* PROPERTY_NAME_SPECIAL_ROOM_MIDPOINT;
        static const char* PROPERTY_NAME_SPECIAL_ROOM_BOSS_CORRIDOR;
        static const char* PROPERTY_NAME_SPECIAL_ROOM_BOSS_CHAMBER;
        static const char* PROPERTY_NAME_BOSS_ENTITY;
        static const char* PROPERTY_NAME_ROOM_ID;
        static const char* PROPERTY_NAME_ROOM_X;
        static const char* PROPERTY_NAME_ROOM_Y;
        static const char* PROPERTY_NAME_ROOM_HERO_SPAWN_X;
        static const char* PROPERTY_NAME_ROOM_HERO_SPAWN_Y;
        static const char* PROPERTY_NAME_ROOM_WIDTH;
        static const char* PROPERTY_NAME_ROOM_HEIGHT;
        static const char* PROPERTY_NAME_ROOM_CAMERABOUNDS;
        static const char* PROPERTY_NAME_ROOM_CAMERABOUNDS_X;
        static const char* PROPERTY_NAME_ROOM_CAMERABOUNDS_Y;
        static const char* PROPERTY_NAME_ROOM_CAMERABOUNDS_WIDTH;
        static const char* PROPERTY_NAME_ROOM_CAMERABOUNDS_HEIGHT;
        static const char* PROPERTY_NAME_ROOM_TILE;
        static const char* PROPERTY_NAME_ROOM_TILEATTRIBUTES;
        static const char* PROPERTY_NAME_ROOM_ENEMIES;
        static const char* PROPERTY_NAME_ROOM_ENEMIES_TYPE;
        static const char* PROPERTY_NAME_ROOM_ENEMIES_POSITION;
        static const char* PROPERTY_NAME_ROOM_ENEMIES_POSITION_X;
        static const char* PROPERTY_NAME_ROOM_ENEMIES_POSITION_Y;
        static const char* PROPERTY_NAME_ROOM_ENEMIES_DIRECTION;
        static const char* PROPERTY_NAME_ROOM_ITEMS;
        static const char* PROPERTY_NAME_ROOM_ITEMS_TYPE;
        static const char* PROPERTY_NAME_ROOM_ITEMS_X;
        static const char* PROPERTY_NAME_ROOM_ITEMS_Y;
        static const char* PROPERTY_NAME_ROOM_TRANSITIONS;
        static const char* PROPERTY_NAME_ROOM_DOORS;
        static const char* PROPERTY_NAME_ROOM_DOORS_X;
        static const char* PROPERTY_NAME_ROOM_DOORS_Y;
        static const char* PROPERTY_NAME_ROOM_DOORS_WIDTH;
        static const char* PROPERTY_NAME_ROOM_DOORS_HEIGHT;
        static const char* PROPERTY_NAME_ROOM_DOORS_ENTRANCE;
        static const char* PROPERTY_NAME_ROOM_DOORS_EXIT;

        static const int DEFAULT_HERO_SPAWN_X;
        static const int DEFAULT_HERO_SPAWN_Y;

        enum SpawnType {
            SPAWN_ENEMY = 1,
            SPAWN_ITEM  = 2
        };

        std::shared_ptr<AnimationSetCache> animationSetCache;
        std::shared_ptr<ImageCache> imageCache;
        std::shared_ptr<TilesetCache> tilesetCache;

        MapPtr constructMap(const Json::Value &json) const;
        RoomPtr constructRoom(const Json::Value &json, int gridSize) const;
        SpawnerPtr constructSpawner(const Json::Value &json, SpawnType type, int offsetX = 0, int offsetY = 0) const;
        std::unique_ptr<Door> constructDoor(const Json::Value & json, int offsetX = 0, int offsetY = 0) const;
        RoomTransition constructTransition(const Json::Value &json) const;
        Rectangle2D<int> constructCameraBounds(const Json::Value &json, 
                int roomX, int roomY, int gridSize) const;
        bool validateMapStructure(const Json::Value &json) const;
        bool validateRoomStructure(const Json::Value &json) const;

    public:
        MapLoader(const std::shared_ptr<AnimationSetCache> & animationSetCache, 
            const std::shared_ptr<ImageCache> & imageCache,
            const std::shared_ptr<TilesetCache> &tilesetCache
        );
        virtual ~MapLoader();
        MapPtr loadFromJson(const Json::Value &json) const;
    };

} // hikari

#endif // HIKARI_CORE_GAME_MAP_MAPLOADER