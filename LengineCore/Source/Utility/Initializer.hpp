#pragma once
#include <vector>
#include <map>

//#include <Utility/ClassTraits.hpp>
#include <Utility/Invoker.hpp>
#include <Utility/Name.hpp>


#define INITIALIZER_EXPAND_PREREQUISITES(...) EXPAND_COMMA_SEPARATED(FOR_EACH(HASH, __VA_ARGS__))

#define DECLARE_INITIALIZER_0(Name) \
	namespace Initializers \
	{ \
	struct On##Name : public Initializer \
	{ \
	    On##Name( Action< EInitializerActionType > a_Action ) \
	        : Initializer( #Name##_H, a_Action, {} ) \
	    {} \
	}; \
	}

#define DECLARE_INITIALIZER_1(Name, ...) \
	namespace Initializers \
	{ \
	struct On##Name : public Initializer \
	{ \
		On##Name( Action< EInitializerActionType > a_Action ) \
			: Initializer( #Name##_N, a_Action, { INITIALIZER_EXPAND_PREREQUISITES(__VA_ARGS__) } ) \
		{} \
	}; \
	}

#define DECLARE_INITIALIZER(...) EXPAND(GET_MACRO_2(__VA_ARGS__, DECLARE_INITIALIZER_1, DECLARE_INITIALIZER_0)(__VA_ARGS__))

#define DEFINE_INITIALIZER(Name) volatile Initializers::On##Name s_On##Name = ( Action< EInitializerActionType > )[]( EInitializerActionType ActionType )

#define DEFINE_INITIALIZER_IN_PLACE_ANONYMOUS_0() \
	struct OnInitialization : public Initializer \
	{ \
		template < typename _Action > \
		On##Initialization( _Action&& a_Action ) \
			: Initializer( ClassTraits< std::decay_t< _Action > >::GetTypeHash(), ( Action< EInitializerActionType > )a_Action, {} ) \
		{} \
	}; \
	inline static OnInitialization s_OnInitialization = []( EInitializerActionType ActionType )

#define DEFINE_INITIALIZER_IN_PLACE_ANONYMOUS_1(...) \
	struct OnInitialization : public Initializer \
	{ \
		template < typename _Action > \
		OnInitialization( _Action&& a_Action ) \
			: Initializer( ClassTraits< std::decay_t< _Action > >::GetTypeHash(), ( Action< EInitializerActionType > )a_Action, { INITIALIZER_EXPAND_PREREQUISITES(__VA_ARGS__) } ) \
		{} \
	}; \
	inline static OnInitialization s_OnInitialization = []( EInitializerActionType ActionType )

#define DEFINE_INITIALIZER_IN_PLACE_ANONYMOUS(...) EXPAND(GET_MACRO_1(__VA_ARGS__, DEFINE_INITIALIZER_IN_PLACE_ANONYMOUS_1, DEFINE_INITIALIZER_IN_PLACE_ANONYMOUS_0)(__VA_ARGS__))

#define DEFINE_INITIALIZER_IN_PLACE_0(Name) \
	struct On##Name : public Initializer \
	{ \
		On##Name( Action< EInitializerActionType > a_Action ) \
			: Initializer( #Name##_N, a_Action, {} ) \
		{} \
	}; \
	inline static On##Name s_On##Name = ( Action< EInitializerActionType > )[]( EInitializerActionType ActionType )

#define DEFINE_INITIALIZER_IN_PLACE_1(Name, ...) \
	struct On##Name : public Initializer \
	{ \
		On##Name( Action< EInitializerActionType > a_Action ) \
			: Initializer( #Name##_N, a_Action, { INITIALIZER_EXPAND_PREREQUISITES(__VA_ARGS__) } ) \
		{} \
	}; \
	inline static On##Name s_On##Name = ( Action< EInitializerActionType > )[]( EInitializerActionType ActionType )

#define DEFINE_INITIALIZER_IN_PLACE(...) EXPAND(GET_MACRO_2(__VA_ARGS__, DEFINE_INITIALIZER_IN_PLACE_1, DEFINE_INITIALIZER_IN_PLACE_0)(__VA_ARGS__))


enum class EInitializerActionType
{
	Teardown,
	Startup
};

class Initializer
{
private:

	struct Node
	{
		Name Name;
		bool Fired = false;
		bool Registered = false;
		Action< EInitializerActionType > Action;
		std::vector< Node* > Requirements;
		std::vector< Node* > Dependants;

		void TriggerStartup();
		void TriggerTeardown();
	};

public:

	static constexpr bool AutoTrigger = false;

	Initializer( const Action<>& a_Action );
	Initializer( const Name& a_Name, const Action< EInitializerActionType >& a_Action, std::initializer_list< hash_t > a_Requirements = {} );

	/// <summary>
	/// Trigger a startup for all registered Initializer's. If a startup has already occured, this will do nothing.
	/// If you set ForceStartup to true and a startup has already occurred, a teardown will happen first.
	/// </summary>
	static void Startup( bool a_ForceStartup = false );

	/// <summary>
	/// Trigger a teardown for all registered Initializer's. If a a teardown has already occured, this will do nothing.
	/// If you set ForceTeardown to true, a teardown will occur regardless.
	/// </summary>
	static void Teardown( bool a_ForceTeardown = false );

	/// <summary>
	/// Check if an Initializer by the given name has just been fired.
	/// </summary>
	static bool HasFired( hash_t a_Name );

	/// <summary>
	/// Check to see if a Startup has already occured as the last action.
	/// </summary>
	static bool IsReady();

private:

	static void Reset();
	static auto& HasReset() { static bool Value = !AutoTrigger; return Value; }
	static auto& GetRoots() { static std::vector< Node* > s_Roots; return s_Roots; }
	static auto& GetNodes() { static std::map< hash_t, Node > s_Nodes; return s_Nodes; }
};