#include <flecs_lua.h>

#include <lualib.h>
#include <lauxlib.h>

ECS_STRUCT(lua_test_struct,
{
    char c;
    char a[4];
    
    uint8_t u8;
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;
    int8_t i8;
    int8_t i16;
    int32_t i32;
    int64_t i64;
    float f32;
    double f64;
});

ECS_DECLARE_COMPONENT(lua_test_struct);


struct vars
{
    lua_test_struct s;
};

static struct vars g;
static struct vars g_out;

static void init_globals(void)
{
    lua_test_struct s =
    {
        .c = 1,
        .u8 = 234,
        .u16 = 234,
        .u32 = 234,
        .u64 = 234,
        .i8 = 234,
        .i16 = 234,
        .i32 = 234,
        .i64 = 234,
        .f32 = 234,
        .f64 = 234,
    };

    memcpy(&g.s, &s, sizeof(s));
}

int lpush_test_struct(lua_State *L)
{
    ecs_world_t *w = ecs_lua_get_world(L);

    lua_pushinteger(L, 232);

 //   ecs_lua_push_ptr(w, ecs_entity(lua_test_struct), &g.s, L);
    return 1;
}

int lset_test_struct(lua_State *L)
{
    ecs_world_t *w = ecs_lua_get_world(L);
    
    return 1;
}

static const luaL_Reg test_lib[] =
{
    { "struct", lpush_test_struct },
    { "set_struct", lset_test_struct },
    { NULL, NULL }
};

int luaopen_test(lua_State *L)
{
    luaL_newlib(L, test_lib);

    return 1;
}

int main(int argc, char **argv)
{
    if(argc < 2) return 1;

    ecs_world_t *w = ecs_init();
    
    ECS_IMPORT(w, FlecsMeta);

    ECS_META(w, lua_test_struct);

    lua_State *L = luaL_newstate();

    luaL_openlibs(L);

    ecs_lua_init(w, L);

    luaL_requiref(L, "test", luaopen_test, 1);
    lua_pop(L, 1);

    int ret = luaL_dofile(L, argv[1]);

    lua_close(L);

    ecs_fini(w);

    return ret;
}