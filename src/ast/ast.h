#ifndef AST_H
#define AST_H

#include "ast/ast_visitor.h"
#include "lexer/token.h"

#include <memory>

namespace ctc::ast
{
    using ctc::lexer::token;

    class Visitor;

    class ASTNode
    {
    public:
        virtual void accept(IVisitor *visitor) = 0;
        virtual ~ASTNode() = default;
    };

    class UsingDecl : ASTNode
    {
        token identifier;
        std::unique_ptr<TypeQualifier> qualifier;

        explicit UsingDecl(token token_of_identifier, std::unique_ptr<TypeQualifier> qualifier_ptr);

        virtual void accept(IVisitor *visitor) override;
    };

    class StorageModifier : ASTNode
    {
        bool is_static, is_extern;

        explicit StorageModifier(bool sm_is_static, bool sm_is_extern);

        virtual void accept(IVisitor *visitor) override;
    };

    class TypeQualifier : ASTNode
    {
        bool is_const, is_volatile;

        explicit TypeQualifier(bool type_qf_is_const, bool type_qf_is_volatile);

        virtual void accept(IVisitor *visitor) override;
    };

    class TypeName : ASTNode
    {
        std::unique_ptr<TypeQualifier> qualifier;
        bool ptr;
        std::unique_ptr<Type> type;

        explicit TypeName(std::unique_ptr<TypeQualifier> qualifier_ptr, bool is_ptr, std::unique_ptr<Type> type_ptr);

        virtual void accept(IVisitor *visitor) override;
    };

    class Type : ASTNode
    {
        token value;

        explicit Type(token value_of_token);

        virtual void accept(IVisitor *visitor) override;
    };
}

#endif
