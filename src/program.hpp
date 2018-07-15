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

#ifndef DIRCONF_PROGRAM_HPP
#define DIRCONF_PROGRAM_HPP

#include <filesystem>
#include <set>
#include <vector>

#include <speed/speed.hpp>
#include <speed/speed_alias.hpp>

#include "file_managers.hpp"


namespace dirconf {


class program
{
public:
    program(
            std::filesystem::path src_pth,
            std::vector<std::string> conf_nmes,
            spd::contain::flags<file_managers> fle_managrs
    );
    
    int execute();
    
private:
    bool execute_in_directory(
            const std::filesystem::path& dir_pth,
            const std::filesystem::path& last_conf_pth
    );
    
    void visit_inode(const std::filesystem::path& dir_pth);
    
    bool is_inode_visited(const std::filesystem::path& dir_pth) const noexcept;
    
    bool apply_configuration(
            const std::filesystem::path& dir_pth,
            const std::filesystem::path& conf_pth,
            bool* recur
    ) const;
    
    bool apply_gvfs_attribute(const char* fle, const char* attr, const char* val) const noexcept;
    
private:
    std::filesystem::path src_pth_;
    
    std::set<std::uint64_t> vistd_inos_;
    
    std::vector<std::string> conf_nmes_;
    
    spd::contain::flags<file_managers> fle_managrs_;
};


}


#endif
