#include "discord_rpc_helper.hpp"
#include "discord/rich_presence.hpp"
#include "ida_helper.hpp"

#include <pro.h>
#include <kernwin.hpp>
#include <idp.hpp>
#include <loader.hpp>
#include <name.hpp>
#include <bytes.hpp>

extern idarpc::discord::RichPresence *rpc;

namespace {
time_t start_time = time(nullptr);
std::string cached_large_image_text;
std::string cached_state;
std::string cached_details;
std::string cached_small_image_key;
std::string cached_small_image_text;
}

std::string get_ida_version_string()
{
    char version[32] = {0};
    get_kernel_version(version, sizeof(version));
    return std::string(version);
}

namespace idarpc::discord_rpc_helper
{
    void update_presence(const discord_helper_spec &spec)
    {
        if (!rpc)
            return;

        DiscordRichPresence presence{};
        presence.startTimestamp = start_time;

        if (idarpc::idahelper::is_ida_home_version())
        {
            presence.largeImageKey = "ida_home";
            cached_large_image_text = "IDA HOME " + get_ida_version_string();
        }
        else
        {
            presence.largeImageKey = "ida_pro";
            cached_large_image_text = "IDA PRO " + get_ida_version_string();
        }
        presence.largeImageText = cached_large_image_text.c_str();

        cached_state = idahelper::get_filename();
        cached_details = spec.details;
        cached_small_image_key = spec.small_image_key;
        cached_small_image_text = spec.small_image_text;

        presence.state = cached_state.empty() ? nullptr : cached_state.c_str();
        presence.details = cached_details.empty() ? nullptr : cached_details.c_str();
        presence.smallImageKey = cached_small_image_key.empty() ? nullptr : cached_small_image_key.c_str();
        presence.smallImageText = cached_small_image_text.empty() ? nullptr : cached_small_image_text.c_str();

        rpc->update_presence(presence);
    }

}
