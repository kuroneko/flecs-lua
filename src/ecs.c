#include <flecs_lua.h>

#include <lualib.h>
#include <lauxlib.h>

ecs_world_t *ecs_lua_get_world(lua_State *L)
{
    lua_pushstring(L, "ecs_world");
    lua_gettable(L, LUA_REGISTRYINDEX);
    ecs_world_t *p = lua_touserdata(L, -1);
    lua_pop(L, 1);
    return p;
}

static int new_entity(lua_State *L)
{
    ecs_world_t *w = ecs_lua_get_world(L);

    ecs_entity_t e = 0;

    switch(lua_gettop(L))
    {
        case 0:
        {
            e = ecs_new(w, 0);
            char num[32];
            snprintf(num, sizeof(num), "Lua.%llu", e);
            ecs_set(w, e, EcsName, {.alloc_value = num});
            // ecs_set(w, e, EcsName, {.value = "Lua.Entity"});
            break;
        }
        case 1:
        {
            //e = luaL_checkinteger(L, 1);
            break;
        }
        default: break;
    }

    lua_pushinteger(L, e);

    return 1;
}

static int delete_entity(lua_State *L)
{
    ecs_world_t *w = ecs_lua_get_world(L);

    ecs_entity_t entity = luaL_checkinteger(L, 1);

    ecs_delete(w, entity);

    return 0;
}

static int entity_name(lua_State *L)
{
    ecs_world_t *w = ecs_lua_get_world(L);

    ecs_entity_t e = luaL_checkinteger(L, 1);

    const char *name = ecs_get_name(w, e);
    
    lua_pushstring(L, name);

    return 1;
}

static int lookup_entity(lua_State *L)
{
    ecs_world_t *w = ecs_lua_get_world(L);

    const char *name = luaL_checkstring(L, 1);

    ecs_entity_t e = ecs_lookup(w, name);

    lua_pushinteger(L, e);

    return 1;
}

static int func(lua_State *L)
{
    ecs_world_t *w = ecs_lua_get_world(L);
    return 1;
}

static const luaL_Reg ecs_lib[] =
{
    { "new", new_entity },
    { "delete", delete_entity },
    { "name", entity_name },
    { "lookup", lookup_entity },
    { NULL, NULL }
};

int luaopen_ecs(lua_State *L)
{
    luaL_newlib(L, ecs_lib);
    return 1;
}

int ecs_lua_init(ecs_world_t *world, lua_State *L)
{
    if(world == NULL || L == NULL) return 1;

    lua_pushstring(L, "ecs_world");
    lua_pushlightuserdata(L, world);
    lua_settable(L, LUA_REGISTRYINDEX);

    //luaL_requiref(L, "_G", luaopen_ecs, 1);
    luaL_requiref(L, "ecs", luaopen_ecs, 1);
    lua_pop(L, 1);


    return 0;
}

int ecs_lua_exit(lua_State *L)
{
    return 0;
}