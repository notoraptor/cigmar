//
// Created by notoraptor on 17-11-23.
//

#ifndef SRC_CIGMAR_MANAGER_HPP
#define SRC_CIGMAR_MANAGER_HPP

#include <cigmar/utils/Initializable.hpp>
#include <cigmar/video/manager/Database.hpp>
#include <cigmar/video/manager/DbVideo.hpp>

namespace cigmar::videomanager {
	typedef video::manager::Database Database;
	typedef video::manager::Library Library;
	typedef video::manager::Folder Folder;
	typedef video::manager::DbVideo DbVideo;
}

#endif //SRC_CIGMAR_MANAGER_HPP
