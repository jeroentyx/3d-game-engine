#include <glm/glm.hpp>
#include "../Src/Engine/Core/Manager/EntityID.h"
#include "Src/Engine/Systems/Graphics/Animation/AnimationWrapModes.h"

using string_t = std::string;

//--------------------------------------------------------------------------------------------
// Some structure to show that you can add your own atomic structures
//--------------------------------------------------------------------------------------------
struct oobb
{
    float m_Min{}, m_Max{};
};

struct script_ref
{
    EntityID m_EntityID;
    std::string Modulename;
};


struct A_State
{
    std::string stateName;
    AnimationWrapMode mode;
    std::string animTrackName;
};


enum  MeshType : int
{
    SPHERE = 0,
    CUBE = 1,
    NONE = 2,
};

enum  ShaderType : int
{
    PHONG = 0,
    SHADOW = 1,
    DEFAULT = 2,
};

//--------------------------------------------------------------------------------------------
// User settings
//--------------------------------------------------------------------------------------------
namespace property
{
    //--------------------------------------------------------------------------------------------
    // These section provides a basic frame work to for an editor
    //--------------------------------------------------------------------------------------------
    namespace editor
    {
        // Specifies custom renders for each type
        struct style_info_base
        {
            void* m_pDrawFn{ nullptr };
        };

        // Settings for each type when rendering
        template< typename T >
        struct style_info : style_info_base {};

        // Custom settings for rendering ints
        template<>
        struct style_info<int> : style_info_base
        {
            int                     m_Min;
            int                     m_Max;
            const char*             m_pFormat;
            float                   m_Speed;
        };

        // Custom settings for rendering floats
        template<>
        struct style_info<float> : style_info_base
        {
            float                   m_Min;
            float                   m_Max;
            const char*             m_pFormat;
            float                   m_Speed;
            float                   m_Power;
        };

        template<>
        struct style_info<string_t> : style_info_base
        {
            enum class style
            {
                  NORMAL
                , ENUM 
            };

            const std::pair<const char*, int>*  m_pEnumList;
            std::size_t                         m_EnumCount;
            style                               m_Style;
        };
    }
    //-----------------------------------------------------------------------------------

    template< typename T >
    struct edstyle
    {
        static editor::style_info<T> Default(void) noexcept;
    };

    //-----------------------------------------------------------------------------------

    template<>
    struct edstyle<int>
    {    
        static editor::style_info<int> ScrollBar(int Min, int Max, const char* pFormat = "%d")                                                                                             noexcept;
        static editor::style_info<int> Drag     (float Speed = 1.0f, int Min = std::numeric_limits<int>::lowest(), int Max = std::numeric_limits<int>::max(), const char* pFormat = "%d")  noexcept;
        static editor::style_info<int> Default  (void) noexcept { return Drag(); }
    };

    //-----------------------------------------------------------------------------------

    template<>
    struct edstyle<float>
    {
        static editor::style_info<float> ScrollBar(float Min, float Max, const char* pFormat = "%.3f", float Power = 1.0f)                                                                                                 noexcept;
        static editor::style_info<float> Drag     (float Speed = 1.0f, float Min = std::numeric_limits<float>::lowest(), float Max = std::numeric_limits<float>::max(), const char* pFormat = "%.3f", float Power = 1.0f)  noexcept;
        static editor::style_info<float> Default  (void) noexcept { return Drag(); }
    };

    //-----------------------------------------------------------------------------------

    template<>
    struct edstyle<string_t>
    {
        template< std::size_t N >
        static editor::style_info<string_t> Enumeration(const std::array<std::pair<const char*, int>, N>& Array)  noexcept;
        static editor::style_info<string_t> Button     (void)                                                     noexcept;
        static editor::style_info<string_t> Default    (void)                                                     noexcept;
    };

    //-----------------------------------------------------------------------------------
    // If we are not including an editor lets give the system some empty functions
    //-----------------------------------------------------------------------------------
#ifndef PROPERTY_EDITOR
    template< typename T >
    __inline editor::style_info<T>             edstyle<T>::            Default     (void)                                              noexcept { return {}; }
    __inline editor::style_info<int>           edstyle<int>::          ScrollBar   (int, int, const char*)                             noexcept { return {}; }
    __inline editor::style_info<int>           edstyle<int>::          Drag        (float, int, int, const char*)                      noexcept { return {}; }
    __inline editor::style_info<float>         edstyle<float>::        ScrollBar   (float, float, const char*, float)                  noexcept { return {}; }
    __inline editor::style_info<float>         edstyle<float>::        Drag        (float, float, float, const char*, float)           noexcept { return {}; }
    template< std::size_t N >
    __inline editor::style_info<string_t>      edstyle<string_t>::     Enumeration (const std::array<std::pair<const char*, int>, N>&) noexcept { return {}; }
    __inline editor::style_info<string_t>      edstyle<string_t>::     Default     (void)                                              noexcept { return {}; }
    __inline editor::style_info<string_t>      edstyle<string_t>::     Button      (void)                                              noexcept { return {}; }
#endif

    //--------------------------------------------------------------------------------------------
    // Settings for the property system
    //--------------------------------------------------------------------------------------------
    namespace settings
    {
        //--------------------------------------------------------------------------------------------
        // The properties which the property system is going to know about.
        //--------------------------------------------------------------------------------------------
        using data_variant = std::variant
        <
              int
            , bool
            , float
			, uint32_t
            , string_t
            , oobb
            , glm::vec2
            , glm::vec3
            , script_ref
            , MeshType
            , ShaderType
            , A_State
        >;
    
        //--------------------------------------------------------------------------------------------
        // Group all the editor::setting_info under a single varient. We must follow data_variant order
        // to stay type safe.
        //--------------------------------------------------------------------------------------------
        namespace editor
        {
            struct empty {};

            // Helper
            namespace details
            {
                template<typename... T>
                std::variant< property::editor::style_info<T> ... , empty> CreateEditorEditStyles( std::variant< T... > );
            }

            // Actual variant with all the different editing styles
            using styles_info_variant = decltype( details::CreateEditorEditStyles( std::declval<data_variant>() ) );
        }

        //--------------------------------------------------------------------------------------------
        // User define data for each property
        //--------------------------------------------------------------------------------------------
        struct user_entry
        {
            const char*                                                 m_pHelp            { nullptr };             // A simple string describing to the editor's user what this property does
            editor::styles_info_variant                                 m_EditStylesInfo   { editor::empty{} };     // If not style is set then the default will be used
            
            constexpr user_entry() = default;

            // Function for user to setup a help string for their properties
            template< typename T = property::setup_entry >                                  // We use a template here to ask the compiler to resolve this function later
            constexpr T Help ( const char* pHelp ) const noexcept                           // Thanks to that we can make this function a constexpr function
            { 
                T r = *static_cast<const T*>(this);                                         // Because we are a constexpr function we can not modify the class directly we must copy it
                r.m_pHelp = pHelp;                                                          // Now we can modify the variable that we care about
                return r;                                                                   // We return our new instance
            }

            // Setting the editor display style (Look in imGuiPropertyExample for mode details)
            template< typename T = property::setup_entry >                                  // We use a template here to ask the compiler to resolve this function later
            constexpr T EDStyle( const editor::styles_info_variant&& Style ) const noexcept // Thanks to that we can make this function a constexpr function
            {
                T r = *static_cast<const T*>(this);
                assert( r.m_FunctionTypeGetSet.index() == Style.index() );                  // Make sure that the property type is the same type
                r.m_EditStylesInfo = std::move(Style);                                      // Call using the constructor to make sure this function can stay constexpr
                return r;
            }
        };
    }
}

