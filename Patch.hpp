//
//  Patch.hpp
//  octopus
//
//  Created by Stijn Frishert on 10/19/16.
//
//

#ifndef OCTOPUS_PATCH_HPP
#define OCTOPUS_PATCH_HPP

#include <memory>
#include <string>
#include <unordered_map>

#include "Node.hpp"

namespace octo
{
    class Patch
    {
    public:
        
    private:
        //! The nodes in the patch
        std::unordered_map<std::string, std::unique_ptr<Node>> nodes;
    };
}

#endif
