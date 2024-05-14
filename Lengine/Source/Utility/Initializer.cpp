#include <Utility/Initializer.hpp>

Initializer::Initializer( const Action<>& a_Action )
{
	if constexpr ( AutoTrigger )
	{
		a_Action();
	}
}

Initializer::Initializer( const Name& a_Name, const Action< EInitializerActionType >& a_Action, const std::initializer_list< hash_t > a_Requirements )
{
	// Try to insert an Initializer node by the name given.
	Node& ThisNode = GetNodes()[ a_Name ];

	if ( !ThisNode.Registered )
	{
		ThisNode.Name = a_Name;
		ThisNode.Registered = true;
		ThisNode.Action = a_Action;
	}

	// Register all requirements if there are requirements. If none, register as a root.
	if ( a_Requirements.size() > 0 )
	{
		for ( hash_t Requirement : a_Requirements )
		{
			Node& RequirementNode = GetNodes()[ Requirement ];
			ThisNode.Requirements.push_back( &RequirementNode );
			RequirementNode.Dependants.push_back( &ThisNode );
		}
	}
	else
	{
		GetRoots().push_back( &ThisNode );
	}

	// If auto trigger is enabled, then cascade a fire event downwards if all requirements have fired.
	if constexpr ( AutoTrigger )
	{
		ThisNode.TriggerStartup();
	}
}

void Initializer::Node::TriggerStartup()
{
	// If we find at least one requirement that has not yet fired, this Initializer cannot fire.
	for ( const auto Requirement : Requirements )
	{
		if ( !Requirement->Fired )
		{
			return;
		}
	}

	// If all requirements have fired, then we are free to activate.
	Action.Invoke( EInitializerActionType::Startup );
	Fired = true;

	// We should then propogate the firing downwards.
	for ( const auto Dependant : Dependants )
	{
		Dependant->TriggerStartup();
	}
}

void Initializer::Node::TriggerTeardown()
{
	// If this node has already been torn down, then return.
	if ( Fired )
	{
		return;
	}

	// If not, wait for all dependents to be torn down first.
	for ( const auto Dependant : Dependants )
	{
		Dependant->TriggerTeardown();
	}

	// ...then trigger the teardown.
	Action.Invoke( EInitializerActionType::Teardown );
	Fired = true;
}

void Initializer::Reset()
{
	for ( auto& Node : GetNodes() )
	{
		Node.second.Fired = false;
	}
}

void Initializer::Startup( const bool a_ForceStartup )
{
	if ( !a_ForceStartup && IsReady() )
	{
		return;
	}

	if ( IsReady() )
	{
		Teardown();
		Reset();
	}

	for ( const auto Root : GetRoots() )
	{
		Root->TriggerStartup();
	}

	HasReset() = false;
}

void Initializer::Teardown( const bool a_ForceTeardown )
{
	if ( !a_ForceTeardown && !IsReady() )
	{
		return;
	}

	Reset();

	for ( const auto Root : GetRoots() )
	{
		Root->TriggerTeardown();
	}

	HasReset() = true;
}

bool Initializer::HasFired( const hash_t a_Name )
{
	const auto Iter = GetNodes().find( a_Name );
	return Iter == GetNodes().end() ? false : Iter->second.Fired;
}

bool Initializer::IsReady()
{
	return !HasReset();
}