/* dirconf
 * Copyright (C) 2018-2023 Killian Valverde.
 *
 * This file is part of dirconf.
 *
 * dirconf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * dirconf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dirconf. If not, see <http://www.gnu.org/licenses/>.
 */

//
// Created by Killian on 15/07/18.
//

#include <gio/gio.h>
#include <glib.h>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "program.hpp"


namespace dirconf {


program::program(
        std::filesystem::path src_pth,
        std::vector<std::string> conf_nmes,
        spd::contain::flags<file_managers> fle_managrs
)
        : src_pth_(std::move(src_pth))
        , conf_nmes_(std::move(conf_nmes))
        , fle_managrs_(std::move(fle_managrs))
        , vistd_inos_()
{
}


int program::execute()
{
    return execute_in_directory(src_pth_, std::filesystem::path()) ? 0 : -1;
}


bool program::execute_in_directory(
        const std::filesystem::path& dir_pth,
        const std::filesystem::path& last_conf_pth
)
{
    std::filesystem::path conf_pth;
    bool found = false;
    bool sucss = true;
    bool recur = false;
    
    visit_inode(dir_pth);
    
    conf_pth = dir_pth;
    conf_pth /= ".";
    
    for (auto& x : conf_nmes_)
    {
        conf_pth.replace_filename(x);
        
        if (spd::sys::fsys::is_regular_file(conf_pth.c_str()))
        {
            found = true;
            break;
        }
    }
    if (!found && !last_conf_pth.empty())
    {
        conf_pth = last_conf_pth;
        found = true;
    }
    if (found)
    {
        std::cout << spd::ios::set_light_blue_text
                  << "Applying configuration " << conf_pth.filename() << " in " << dir_pth;
        
        if (!apply_configuration(dir_pth, conf_pth, &recur))
        {
            sucss = false;
            std::cout << spd::ios::set_light_red_text << " [fail]" << spd::ios::newl;
        }
        else
        {
            std::cout << spd::ios::set_light_green_text << " [ok]" << spd::ios::newl;
        }
    }
    
    try
    {
        for (auto& x : std::filesystem::directory_iterator(dir_pth))
        {
            if (std::filesystem::is_directory(x) && !is_inode_visited(x))
            {
                if (!execute_in_directory(x.path(), recur ? conf_pth : std::filesystem::path()))
                {
                    sucss = false;
                }
            }
        }
    }
    catch (const std::filesystem::filesystem_error& fe)
    {
        std::cerr << spd::ios::set_light_red_text
                  << "Error executing in directory: " << dir_pth
                  << spd::ios::newl;
        
        return false;
    }
    
    return sucss;
}


void program::visit_inode(const std::filesystem::path& dir_pth)
{
    vistd_inos_.insert(spd::sys::fsys::get_file_inode(dir_pth.c_str()));
}


bool program::is_inode_visited(const std::filesystem::path& dir_pth) const noexcept
{
    return vistd_inos_.count(spd::sys::fsys::get_file_inode(dir_pth.c_str())) > 0;
}


bool program::apply_configuration(
        const std::filesystem::path& dir_pth,
        const std::filesystem::path& conf_pth,
        bool* recur
) const
{
    namespace pt = boost::property_tree;
    
    bool sucss = true;
    *recur = false;
    
    try
    {
        pt::ptree root;
        std::string icon_ptah;
        std::string view;
        std::size_t icon_view_zoom_level;
        std::size_t list_view_zoom_level;
        bool compact_layaut;
        const char* val;
        
        pt::read_json(conf_pth, root);
        
        icon_ptah = root.get<std::string>("icon-path", "");
        view = root.get<std::string>("view", "");
        icon_view_zoom_level = root.get<std::size_t>("icon-view-zoom-level", 0);
        list_view_zoom_level = root.get<std::size_t>("list-view-zoom-level", 0);
        compact_layaut = root.get<bool>("compact-layout", false);
        *recur = root.get<bool>("apply-recursively", false);
        
        if (!icon_ptah.empty())
        {
            if (!apply_gvfs_attribute(dir_pth.c_str(), "metadata::custom-icon", icon_ptah.c_str()))
            {
                sucss = false;
            }
        }
        
        if (!view.empty())
        {
            if (view == "icons")
            {
                val = "OAFIID:Nemo_File_Manager_Icon_View";
            }
            else if (view == "compact")
            {
                val = "OAFIID:Nemo_File_Manager_Compact_View";
            }
            else
            {
                sucss = false;
                goto end_view;
            }
            
            if (!apply_gvfs_attribute(dir_pth.c_str(), "metadata::nemo-default-view", val))
            {
                sucss = false;
            }
        }
        end_view:
        
        if (icon_view_zoom_level != 0)
        {
            std::string str_zoom_levl = std::to_string(icon_view_zoom_level);
            if (!apply_gvfs_attribute(dir_pth.c_str(), "metadata::nemo-icon-view-zoom-level",
                                      str_zoom_levl.c_str()))
            {
                sucss = false;
            }
        }
    
        if (list_view_zoom_level != 0)
        {
            std::string str_zoom_levl = std::to_string(list_view_zoom_level);
            if (!apply_gvfs_attribute(dir_pth.c_str(), "metadata::nemo-list-view-zoom-level",
                                      str_zoom_levl.c_str()))
            {
                sucss = false;
            }
        }
        
        std::string str_compact_layt = compact_layaut ? "true" : "false";
        if (!apply_gvfs_attribute(dir_pth.c_str(), "metadata::nemo-icon-view-auto-layout",
                                  str_compact_layt.c_str()))
        {
            sucss = false;
        }
        if (!apply_gvfs_attribute(dir_pth.c_str(), "metadata::nemo-icon-view-tighter-layout",
                                  str_compact_layt.c_str()))
        {
            sucss = false;
        }
    }
    catch (const pt::json_parser::json_parser_error& jpe)
    {
        return false;
    }
    
    return sucss;
}


bool program::apply_gvfs_attribute(
        const char* fle,
        const char* attr,
        const char* val
) const noexcept
{
    GFile* gfle;
    GFileAttributeType typ;
    gpointer gval;
    GError* err = nullptr;
    
    gfle = g_file_new_for_commandline_arg(fle);
    typ = G_FILE_ATTRIBUTE_TYPE_STRING;
    gval = (void*)val;
    
    if (!g_file_set_attribute(gfle, attr, typ, gval, G_FILE_QUERY_INFO_NONE, nullptr, &err))
    {
        g_error_free(err);
        return false;
    }
    
    return true;
}


}
