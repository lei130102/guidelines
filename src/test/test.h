#pragma once

namespace test_namespace_defer
{
//    struct _on_scope_exit_creator
//    {
//        template<class _Fty>
//        _on_scope_exit<typename std::decay<_Fty>::type> operator << (_Fty&& fn)
//        {
//            return _on_scope_exit<typename std::decay<_Fty>::type>(std::forward<_Fty>(fn));
//        }
//    };
//#define ON_SCOPE_EXIT auto SCOPE_EXIT_TOKEN_CAT__(_on_scope_exit_var_, __LINE__) = VFX::_on_scope_exit_creator() << [&]

    //ÓÃ·¨£º
    //    ImGui::PushID(reinterpret_cast<int64_t>(placer));
    //ON_SCOPE_EXIT{ ImGui::PopID(); };

}