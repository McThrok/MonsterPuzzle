// This class is based on 
// https://stackoverflow.com/questions/44105058/how-does-unitys-getcomponent-work?fbclid=IwAR2H6nTXS-XHkxpqDxaf2sFOXjksJwE0VZbaA5LChQXW94h3YolGhSgFH6A

#pragma once

namespace fe
{
#define TO_STRING( x ) #x

	//****************
	// CLASS_DECLARATION
	//
	// This macro must be included in the declaration of any subclass of Component.
	// It declares variables used in type checking.
	//****************
#define CLASS_DECLARATION( classname )                                                      \
public:                                                                                     \
    static const std::size_t Type;                                                          \
    virtual bool IsClassType( const std::size_t classType ) const override;                 \

//****************
// CLASS_DEFINITION
// 
// This macro must be included in the class definition to properly initialize 
// variables used in type checking. Take special care to ensure that the 
// proper parentclass is indicated or the run-time type information will be
// incorrect. Only works on single-inheritance RTTI.
//****************
#define CLASS_DEFINITION( parentclass, childclass )                                         \
const std::size_t childclass::Type = std::hash< std::string >()( TO_STRING( childclass ) ); \
bool childclass::IsClassType( const std::size_t classType ) const {                         \
        if ( classType == childclass::Type )                                                \
            return true;                                                                    \
        return parentclass::IsClassType( classType );                                       \
}

	class GameObject;

	class Component
	{
	public:
		Component();
		virtual ~Component() = default;

		static const size_t Type;
		virtual bool IsClassType(const size_t classType) const;

		GameObject& GetOwner() const;
		void SetOwner(GameObject& newOwner);
	protected:
		GameObject* owner;
	};
}