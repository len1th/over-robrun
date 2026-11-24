/* menu_bridge.h - Shared settings schema and menu bridge API.
 *
 * BR_SETTINGS is the single source of truth for declarations, defaults,
 * validation and the WebView state payload. The implementation lives in
 * rr_config.c; the WebView host lives in menu_webview.cpp.
 */
#ifndef MENU_BRIDGE_H
#define MENU_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

#define BR_MAGIC 0x57444D31u
#define BR_VERSION 19u
#define BR_COLOR_COUNT 15

/* Shared schema for the WebView, defaults and on-disk settings.
 * Each entry is a field, default, minimum and maximum. */
#define BR_SETTINGS(X) \
    X(enabled, 1, 0, 1) \
    X(menu_key, 0x70, 1, 254) \
    X(stream_proof, 1, 0, 1) \
    X(show_box, 1, 0, 1) \
    X(show_skeleton, 1, 0, 1) \
    X(show_name, 1, 0, 1) \
    X(show_armor, 1, 0, 1) \
    X(show_distance, 1, 0, 1) \
    X(show_weapon, 1, 0, 1) \
    X(show_head, 1, 0, 1) \
    X(show_self, 0, 0, 1) \
    X(show_clan, 1, 0, 1) \
    X(show_police, 1, 0, 1) \
    X(cham_on, 0, 0, 1) \
    X(max_distance, 30000, 0, 200000) \
    X(skel_distance, 15000, 0, 200000) \
    X(line_w, 1, 1, 4) \
    X(radar_on, 1, 0, 1) \
    X(radar_range, 150, 25, 500) \
    X(aa_on, 0, 0, 1) \
    X(aa_show_fov, 1, 0, 1) \
    X(aa_fov_px, 150, 30, 600) \
    X(aa_key, 0x12, 1, 254) \
    X(aa_smooth, 5, 1, 10) \
    X(aa_aim_cal, 100, 10, 2000) \
    X(aa_dist, 0, 0, 1000) \
    X(aa_target_clan, 0, 0, 1) \
    X(aa_target_police, 0, 0, 1) \
    X(aa_bone, 0, 0, 9) \
    X(aa_los_on, 0, 0, 1) \
    X(aa_ignore_knocked, 0, 0, 1) \
    X(aa_closest_first, 0, 0, 1) \
    X(show_loot, 0, 0, 1) \
    X(show_loot_nodes, 0, 0, 1) \
    X(loot_tier_none, 1, 0, 1) \
    X(loot_tier_common, 1, 0, 1) \
    X(loot_tier_uncommon, 1, 0, 1) \
    X(loot_tier_rare, 1, 0, 1) \
    X(loot_tier_epic, 1, 0, 1) \
    X(loot_tier_legendary, 1, 0, 1) \
    X(loot_tier_exotic, 1, 0, 1) \
    X(vehicle_chams, 0, 0, 1) \
    X(show_containers, 0, 0, 1) \
    X(show_doors, 0, 0, 1) \
    X(show_dropped_loot, 0, 0, 1) \
    X(show_vehicle_loot, 0, 0, 1) \
    X(radar_world, 0, 0, 1) \
    X(show_crouch, 0, 0, 1) \
    X(show_health, 1, 0, 1) \
    X(show_vehicle_markers, 0, 0, 1) \
    X(show_vehicle_occupants, 0, 0, 1) \
    X(tb_on, 0, 0, 1) \
    X(tb_key, 0x05, 1, 254) \
    X(tb_bone, 2, 0, 9) \
    X(tb_px, 8, 1, 200) \
    X(tb_delay_ms, 0, 0, 500) \
    X(tb_los_on, 0, 0, 1) \
    X(rc_on, 0, 0, 1) \
    X(rc_key, 0, 0, 254) \
    X(rc_strength, 75, 0, 100) \
    X(rc_smooth, 3, 1, 10) \
    X(rc_reset_ms, 400, 50, 2000) \
    X(show_offset_status, 0, 0, 1) \
    X(show_health_bar, 1, 0, 1) \
    X(show_armor_bar, 1, 0, 1) \
    X(custom_fov_on, 0, 0, 1) \
    X(custom_fov_val, 90, 60, 150) \
    X(show_vehicle_speed, 1, 0, 1) \
    X(speedometer_hud, 1, 0, 1) \
    X(speedometer_pos, 0, 0, 3) \
    X(speedometer_x, 3, 0, 100) \
    X(speedometer_y, 78, 0, 100) \
    X(speedometer_scale, 100, 50, 150) \
    X(sky_color_on, 0, 0, 1) \
    X(sky_color_opacity, 60, 5, 100) \
    X(show_inv_value, 1, 0, 1) \
    X(inv_value_min, 0, 0, 1000000) \
    X(show_vehicle_value, 1, 0, 1) \
    X(head_scale_x, 100, 50, 300) \
    X(head_scale_y, 100, 50, 300) \
    X(stretch_on, 0, 0, 1) \
    X(stretch_factor, 133, 100, 200)

typedef struct BR_Config {
    unsigned int magic, version;
#define BR_DECLARE(field, initial, minimum, maximum) int field;
    BR_SETTINGS(BR_DECLARE)
#undef BR_DECLARE
    struct { unsigned char rgb[3]; } color[BR_COLOR_COUNT];

    int menu_open;
} BR_Config;

/* Unrecognized values use the same no-rarity bucket as the loot renderer. */
static __inline int br_loot_rarity_visible(const BR_Config* config, int rarity) {
    switch (rarity) {
        case 1: return config->loot_tier_common;
        case 2: return config->loot_tier_uncommon;
        case 3: return config->loot_tier_rare;
        case 4: return config->loot_tier_epic;
        case 5: return config->loot_tier_legendary;
        case 6: return config->loot_tier_exotic;
        default: return config->loot_tier_none;
    }
}

BR_Config* br_get(void);
int br_config_set(BR_Config* config, const char* key, int value);
int br_config_save(void);
int br_config_load(void);
int br_config_export(void* owner_hwnd);
int br_config_import(void* owner_hwnd);

int br_start_menu(void);
void br_stop_menu(void);
void br_set_menu_visible(int visible);
void br_request_toggle(void);
void br_play_intro(void);

#ifdef __cplusplus
}
#endif
#endif
