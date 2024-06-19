#pragma once
#include <type_traits>
#include <memory>

using std::shared_ptr;
using std::make_shared;

//==========================================================================
// Singleton class for single-instance objects. If ExplicitInitialisation is
// disabled, T must be default constructible, otherwise it will need to be
// constructed beforehand.
//==========================================================================
template < typename T, bool _ExplicitInitialisation = true >
class ISingleton
{
protected:

    virtual ~ISingleton() = default;

public:

    //======================================================================
    static constexpr bool IsExplicitlyInitialisable = _ExplicitInitialisation;

    //======================================================================
    // Is this singleton valid, that is, is it enabled.
    virtual bool IsValidSingleton() const { return true; }

    //======================================================================
    // This function is fired when an existing or new singleton is bound or constructed.
    virtual void OnPostSingletonConstructed() {}

    //======================================================================
    // Creates the singleton's global instance, taking in constructor args.
    template< typename ...Args >
    static bool Construct( Args&&... a_Args )
    {
        if ( !ASSERT( !s_Instance ) )
        {
            return false;
        }

        s_Instance = make_shared< T >( std::forward< Args >( a_Args )... );
        s_Instance->OnPostSingletonConstructed();
        return true;
    }

    //======================================================================
    // Cause this singleton interface to reference an external instance constructed via other means.
    // Useful for objects created by a global database, where only 1 instance of T is guaranteed.
    static bool BindExisting( T* a_Instance )
    {
        if ( !ASSERT( !s_Instance ) )
        {
            return false;
        }

        s_Instance = shared_ptr< T >( a_Instance );
        s_Instance->OnPostSingletonConstructed();
        return true;
    }

    //======================================================================
    // Will sever reference to existing singleton object without deleting it first.
    static bool UnbindExisting()
    {
        if ( !ASSERT( s_Instance ) )
        {
            return false;
        }

        s_Instance = nullptr;
        return true;
    }

    //======================================================================
    // Destroy the global instance and reset to null. Construct/BindExisting must be called again if explicitly initialisable.
    static void Destroy()
    {
        if ( !s_Instance )
        {
            return;
        }

        s_Instance = nullptr;
    }

    //======================================================================
    // Does the global instance exist and is it valid?
    static bool IsValid() { return s_Instance && s_Instance->IsValidSingleton(); }

    //======================================================================
    // Get a reference to the global instance for this singleton. If not explicitly initialisable, then an instance will be created
    // via default-construction.
    static T* Get()
	{
		if constexpr ( !IsExplicitlyInitialisable )
		{
			if ( !s_Instance )
			{
				Construct();
			}
		}

		CHECK( IsValid() );
		return s_Instance.get();
	}

    //======================================================================
    // Same as Get()
    static struct Access
    {
        T& operator*() const { return *Get(); }
        T* operator->() const { return Get(); }
    } This;

private:

    //======================================================================
    static shared_ptr< T > s_Instance;
};

//==========================================================================
template< typename T, bool _ExplicitInitialisation >
shared_ptr< T > ISingleton< T, _ExplicitInitialisation >::s_Instance = nullptr;

template< typename T, bool _ExplicitInitialisation >
typename ISingleton< T, _ExplicitInitialisation >::Access ISingleton< T, _ExplicitInitialisation >::This;