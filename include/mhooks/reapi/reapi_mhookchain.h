/*
 *  Copyright (C) 2020 the_hunter
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#if defined(HAS_CSSDK_LIB) && defined(HAS_CORE_LIB)
#include <cssdk/public/os_defs.h>
#include <mhooks/common/mhookchain.h>

namespace mhooks
{
    template <typename OrigChain, typename Signature>
    class ReApiMHookChain;

    template <typename OrigChain, typename Ret, typename... Args>
    class ReApiMHookChain<OrigChain, Ret(Args...)> final : public MHookChain<ReApiMHookChain<OrigChain, Ret(Args...)>, Ret, Args...>
    {
        using MHookChainType = MHookChain<ReApiMHookChain<OrigChain, Ret(Args...)>, Ret, Args...>;
        using HooksContainerType = typename MHookChainType::HooksContainerType;
        using HooksContainerSizeType = typename MHookChainType::HooksContainerSizeType;

        template <typename, typename, typename...>
        friend class MHookChain;

        OrigChain*& orig_chain_;
        HooksContainerType enabled_hooks_{};
        HooksContainerType disabled_hooks_{};
        mutable HooksContainerSizeType current_{};

    public:
        /**
         * @brief Constructor.
         *
         * @param orig_chain The original hook chain.
        */
        explicit ReApiMHookChain(OrigChain*& orig_chain)
            : orig_chain_(orig_chain)
        {
        }

        /**
         * @brief Invokes the next hook callback in the chain.
        */
        ATTR_OPTIMIZE_HOT_INLINE Ret CallNext(Args... args) const
        {
            if (current_ < enabled_hooks_.size()) {
                return enabled_hooks_[current_++].second(*this, args...);
            }

            return orig_chain_->CallNext(args...);
        }

        /**
         * @brief Invokes the original function.
        */
        ATTR_OPTIMIZE_HOT_INLINE Ret CallOriginal(Args... args) const
        {
            return orig_chain_->CallOriginal(args...);
        }
    };
}
#endif
