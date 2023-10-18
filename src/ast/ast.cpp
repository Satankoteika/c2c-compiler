#include "ast.h"

#include <cassert>

namespace ctc::ast
{
    UsingDecl::UsingDecl(token token_of_identifier, std::unique_ptr<TypeQualifier> qualifier_ptr) :
                                                                                        identifier(token_of_identifier),
                                                                                        qualifier(std::move(qualifier_ptr))

    {
        assert(qualifier_ptr != nullptr);
    }
    void UsingDecl::accept(IVisitor *visitor) { visitor->visit(this); }

    StorageModifier::StorageModifier(bool sm_is_static, bool sm_is_extern) : is_static(sm_is_static), is_extern(sm_is_extern) {}
    void StorageModifier::accept(IVisitor *visitor) { visitor->visit(this); }

    TypeQualifier::TypeQualifier(bool type_qf_is_const, bool type_qf_is_volatile) : is_const(type_qf_is_const), is_volatile(type_qf_is_volatile) {}
    void TypeQualifier::accept(IVisitor *visitor) { visitor->visit(this); }

    TypeName::TypeName(std::unique_ptr<TypeQualifier> qualifier_ptr, bool is_ptr, std::unique_ptr<Type> type_ptr) :
                                                                                                     qualifier(std::move(qualifier_ptr)),
                                                                                                     ptr(is_ptr),
                                                                                                     type(std::move(type_ptr))

    {
        assert(qualifier_ptr != nullptr && type_ptr != nullptr);
    }
    void TypeName::accept(IVisitor *visitor) { visitor->visit(this); }


    Type::Type(token value_of_token) : value(value_of_token)  {}
    void Type::accept(IVisitor *visitor) { visitor->visit(this); }
}
