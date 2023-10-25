#ifndef AST_H
#define AST_H

#include "ast/ast_visitor.h"
#include "lexer/token.h"

#include <memory>
#include <utility>
#include <vector>

namespace ctc::ast
{
    using ctc::lexer::token;
    using AssignExpression = AssignmentExpression;

    class ASTNode
    {
      public:
        virtual void accept(IASTVisitor *visitor) = 0;
        virtual ~ASTNode() = default;
    };

    class PrimaryExpression : ASTNode
    {
      public:
        std::unique_ptr<token> token_ptr; // Identifier | Number Literal | String Literal
        std::unique_ptr<Expression> expression_ptr;

        explicit PrimaryExpression(std::unique_ptr<token> token,
                                   std::unique_ptr<Expression> expression);

        virtual void accept(IASTVisitor *visitor) override;
    };

    class Expression : ASTNode
    {
      public:
        std::vector<AssignExpression> assign_expr;

        virtual void accept(IASTVisitor *visitor) override;
    };

    class CastExpression : ASTNode
    {
      public:
        std::unique_ptr<TypeName> typename_ptr;
        std::unique_ptr<CastExpression> cast_expression_ptr;

        explicit CastExpression(std::unique_ptr<TypeName> type_name,
                                std::unique_ptr<CastExpression> cast_expression);

        virtual void accept(IASTVisitor *visitor) override;
    };

    class MultiplicativeExpression : ASTNode
    {
      public:
        std::unique_ptr<CastExpression> cast_expression_ptr;
        std::vector<std::pair<token, std::unique_ptr<CastExpression>>> cast_expressions_ptr;

        explicit MultiplicativeExpression(std::unique_ptr<CastExpression> cast_expression);

        virtual void accept(IASTVisitor *visitor) override;
    };

    class AdditiveExpression : ASTNode
    {
      public:
        std::unique_ptr<MultiplicativeExpression> multiplicative_expression_ptr;
        std::vector<std::pair<token, std::unique_ptr<MultiplicativeExpression>>>
            multiplicative_expressions_ptr;

        explicit AdditiveExpression(
            std::unique_ptr<MultiplicativeExpression> multiplicative_expression);

        virtual void accept(IASTVisitor *visitor) override;
    };

    class ShiftExpression : ASTNode
    {
      public:
        std::unique_ptr<AdditiveExpression> additive_expression_ptr;
        std::vector<std::pair<token, std::unique_ptr<AdditiveExpression>>> additive_expressions_ptr;

        explicit ShiftExpression(std::unique_ptr<AdditiveExpression> additive_expression);

        virtual void accept(IASTVisitor *visitor) override;
    };

    class RelationalExpression : ASTNode
    {
      public:
        std::unique_ptr<ShiftExpression> shift_expression_ptr;
        std::vector<std::pair<token, std::unique_ptr<ShiftExpression>>> shift_expressions_ptr;

        explicit RelationalExpression(std::unique_ptr<ShiftExpression> shift_expression);

        virtual void accept(IASTVisitor *visitor) override;
    };

    class EqualityExpression : ASTNode
    {
      public:
        std::unique_ptr<RelationalExpression> relational_expression_ptr;
        std::vector<std::pair<token, std::unique_ptr<RelationalExpression>>>
            relational_expressions_ptr;

        explicit EqualityExpression(std::unique_ptr<RelationalExpression> relational_expression);

        virtual void accept(IASTVisitor *visitor) override;
    };

    class AndExpression : ASTNode
    {
      public:
        std::unique_ptr<EqualityExpression> equality_expression_ptr;
        std::vector<std::unique_ptr<EqualityExpression>> equality_expressions_ptr;

        explicit AndExpression(std::unique_ptr<EqualityExpression> equality_expression);

        virtual void accept(IASTVisitor *visitor) override;
    };

    class ExclusiveOrExpression : ASTNode
    {
      public:
        std::unique_ptr<AndExpression> and_expression_ptr;
        std::vector<std::unique_ptr<AndExpression>> and_expressions_ptr; // abc ^ abc ^ abc

        explicit ExclusiveOrExpression(std::unique_ptr<AndExpression> and_expression);

        virtual void accept(IASTVisitor *visitor) override;
    };

    class InclusiveOrExpression : ASTNode
    {
      public:
        std::unique_ptr<ExclusiveOrExpression> exclusive_or_expression_ptr;
        std::vector<std::unique_ptr<ExclusiveOrExpression>>
            exclusive_or_expressions_ptr; // abc | abc | abc

        explicit InclusiveOrExpression(
            std::unique_ptr<ExclusiveOrExpression> exclusive_or_expression = nullptr);

        virtual void accept(IASTVisitor *visitor) override;
    };

    class LogicalAndExpression : ASTNode
    {
      public:
        std::unique_ptr<InclusiveOrExpression> inclusive_or_expression_ptr;
        std::vector<std::unique_ptr<InclusiveOrExpression>>
            inclusive_or_expressions_ptr; // abc && abc && abc

        explicit LogicalAndExpression(
            std::unique_ptr<InclusiveOrExpression> inclusive_or_expression);

        virtual void accept(IASTVisitor *visitor) override;
    };

    class LogicalOrExpression : ASTNode
    {
      public:
        std::unique_ptr<LogicalAndExpression> logical_and_expression_ptr;
        std::vector<std::unique_ptr<LogicalAndExpression>> and_expressions_ptr; // abc || abc || abc

        explicit LogicalOrExpression(std::unique_ptr<LogicalAndExpression> logical_and_expression);

        virtual void accept(IASTVisitor *visitor) override;
    };

    class ConditionalExpression : ASTNode
    {
      public:
        std::unique_ptr<LogicalOrExpression> logical_or_expression_ptr;

        std::unique_ptr<Expression> then_ptr;
        std::unique_ptr<ConditionalExpression> else_ptr;

        explicit ConditionalExpression(
            std::unique_ptr<LogicalOrExpression> logical_or_expression = nullptr,
            std::unique_ptr<Expression> then = nullptr,
            std::unique_ptr<ConditionalExpression> else_p = nullptr);

        virtual void accept(IASTVisitor *visitor) override;
    };

    class AssignmentExpression : ASTNode
    {
      public:
        std::unique_ptr<ConditionalExpression> сonditional_expression_ptr;

        std::unique_ptr<UnaryExpression> unary_expression_ptr;
        std::unique_ptr<token> assignment_operator_ptr;
        std::unique_ptr<AssignmentExpression> assignment_expression_ptr;

        explicit AssignmentExpression(
            std::unique_ptr<ConditionalExpression> сonditional_expression = nullptr,

            std::unique_ptr<UnaryExpression> unary_expression = nullptr,
            std::unique_ptr<token> assignment_operator = nullptr,
            std::unique_ptr<AssignmentExpression> assignment_expression = nullptr);

        virtual void accept(IASTVisitor *visitor) override;
    };

    class PostfixExpression : ASTNode
    {
      public:
        std::unique_ptr<PrimaryExpression> primary_expr_ptr;

        std::unique_ptr<Expression> expression_for_get_element_by_index_ptr;
        std::unique_ptr<ArgumentExpressionList> function_args_for_call_ptr;

        std::unique_ptr<token> prefix_operator_ptr;
        std::unique_ptr<token> identifier_ptr;
        std::unique_ptr<token> postfix_operator_ptr;

        explicit PostfixExpression(
            std::unique_ptr<PrimaryExpression> primary_expr = nullptr,
            std::unique_ptr<Expression> expression_for_get_element_by_index = nullptr,
            std::unique_ptr<ArgumentExpressionList> function_args_for_call = nullptr,

            std::unique_ptr<token> prefix_operator = nullptr,
            std::unique_ptr<token> identifier = nullptr,
            std::unique_ptr<token> postfix_operator = nullptr);

        virtual void accept(IASTVisitor *visitor) override;
    };

    class ArgumentExpressionList : ASTNode
    {
      public:
        std::vector<std::unique_ptr<AssignmentExpression>> list;

        explicit ArgumentExpressionList()
        {
        }

        virtual void accept(IASTVisitor *visitor) override;
    };

    class UnaryExpression : ASTNode
    {
      public:
        std::unique_ptr<token> prefix_operator;

        std::unique_ptr<PostfixExpression> postfix_ptr;

        std::unique_ptr<UnaryOperator> unary_operator_ptr;
        std::unique_ptr<CastExpression> cast_expression_ptr;

        std::unique_ptr<TypeQualifier> type_qualifier_ptr;

        /*
            unique_ptr& operator=(nullptr) noexcept;
        */
        explicit UnaryExpression(std::unique_ptr<token> prefix = nullptr,
                                 std::unique_ptr<PostfixExpression> postfix = nullptr,
                                 std::unique_ptr<UnaryOperator> unary_operator = nullptr,
                                 std::unique_ptr<CastExpression> cast_expression = nullptr,
                                 std::unique_ptr<TypeQualifier> type_qualifier = nullptr);

        virtual void accept(IASTVisitor *visitor) override;
    };

    class UnaryOperator : ASTNode
    {
      public:
        token value;

        explicit UnaryOperator(token op);

        virtual void accept(IASTVisitor *visitor) override;
    };

    class UsingDecl : ASTNode
    {
      public:
        token identifier;
        std::unique_ptr<TypeQualifier> qualifier;

        explicit UsingDecl(token token_of_identifier, std::unique_ptr<TypeQualifier> qualifier_ptr);

        virtual void accept(IASTVisitor *visitor) override;
    };

    class StorageModifier : ASTNode
    {
      public:
        bool is_static, is_extern;

        explicit StorageModifier(bool sm_is_static, bool sm_is_extern);

        virtual void accept(IASTVisitor *visitor) override;
    };

    class TypeQualifier : ASTNode
    {
      public:
        bool is_const, is_volatile;

        explicit TypeQualifier(bool type_qf_is_const, bool type_qf_is_volatile);

        virtual void accept(IASTVisitor *visitor) override;
    };

    class TypeName : ASTNode
    {
      public:
        std::unique_ptr<TypeQualifier> qualifier;
        bool ptr;
        std::unique_ptr<Type> type;

        explicit TypeName(std::unique_ptr<TypeQualifier> qualifier_ptr, bool is_ptr,
                          std::unique_ptr<Type> type_ptr);

        virtual void accept(IASTVisitor *visitor) override;
    };

    class Type : ASTNode
    {
      public:
        token value;

        explicit Type(token value_of_token);

        virtual void accept(IASTVisitor *visitor) override;
    };
} // namespace ctc::ast

#endif
