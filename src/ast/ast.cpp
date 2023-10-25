#include "ast.h"

#include <cassert>

namespace ctc::ast
{
    PrimaryExpression::PrimaryExpression(std::unique_ptr<token> token,
                                         std::unique_ptr<Expression> expression)
        : token_ptr(std::move(token)), expression_ptr(std::move(expression))
    {
    }
    void PrimaryExpression::accept(IASTVisitor *visitor)
    {
        visitor->visit(this);
    }

    void Expression::accept(IASTVisitor *visitor)
    {
        visitor->visit(this);
    }

    CastExpression::CastExpression(std::unique_ptr<TypeName> type_name,
                                   std::unique_ptr<CastExpression> cast_expression)
        : typename_ptr(std::move(type_name)), cast_expression_ptr(std::move(cast_expression))

    {
    }
    void CastExpression::accept(IASTVisitor *visitor)
    {
        visitor->visit(this);
    }

    MultiplicativeExpression::MultiplicativeExpression(
        std::unique_ptr<CastExpression> cast_expression)
        : cast_expression_ptr(std::move(cast_expression))
    {
    }
    void MultiplicativeExpression::accept(IASTVisitor *visitor)
    {
        visitor->visit(this);
    }

    AdditiveExpression::AdditiveExpression(
        std::unique_ptr<MultiplicativeExpression> multiplicative_expression)
        : multiplicative_expression_ptr(std::move(multiplicative_expression))
    {
    }
    void AdditiveExpression::accept(IASTVisitor *visitor)
    {
        visitor->visit(this);
    }

    ShiftExpression::ShiftExpression(std::unique_ptr<AdditiveExpression> additive_expression)
        : additive_expression_ptr(std::move(additive_expression))
    {
    }
    void ShiftExpression::accept(IASTVisitor *visitor)
    {
        visitor->visit(this);
    }

    RelationalExpression::RelationalExpression(std::unique_ptr<ShiftExpression> shift_expression)
        : shift_expression_ptr(std::move(shift_expression))
    {
    }
    void RelationalExpression::accept(IASTVisitor *visitor)
    {
        visitor->visit(this);
    }

    EqualityExpression::EqualityExpression(
        std::unique_ptr<RelationalExpression> relational_expression)
        : relational_expression_ptr(std::move(relational_expression))
    {
    }
    void EqualityExpression::accept(IASTVisitor *visitor)
    {
        visitor->visit(this);
    }

    AndExpression::AndExpression(std::unique_ptr<EqualityExpression> equality_expression)
        : equality_expression_ptr(std::move(equality_expression))
    {
    }
    void AndExpression::accept(IASTVisitor *visitor)
    {
        visitor->visit(this);
    }

    ExclusiveOrExpression::ExclusiveOrExpression(std::unique_ptr<AndExpression> and_expression)
        : and_expression_ptr(std::move(and_expression))
    {
    }
    void ExclusiveOrExpression::accept(IASTVisitor *visitor)
    {
        visitor->visit(this);
    }

    InclusiveOrExpression::InclusiveOrExpression(
        std::unique_ptr<ExclusiveOrExpression> exclusive_or_expression)
        : exclusive_or_expression_ptr(std::move(exclusive_or_expression))
    {
    }
    void InclusiveOrExpression::accept(IASTVisitor *visitor)
    {
        visitor->visit(this);
    }

    LogicalAndExpression::LogicalAndExpression(
        std::unique_ptr<InclusiveOrExpression> inclusive_or_expression)
        : inclusive_or_expression_ptr(std::move(inclusive_or_expression))
    {
    }
    void LogicalAndExpression::accept(IASTVisitor *visitor)
    {
        visitor->visit(this);
    }

    LogicalOrExpression::LogicalOrExpression(
        std::unique_ptr<LogicalAndExpression> logical_and_expression)
        : logical_and_expression_ptr(std::move(logical_and_expression))
    {
    }
    void LogicalOrExpression::accept(IASTVisitor *visitor)
    {
        visitor->visit(this);
    }

    ConditionalExpression::ConditionalExpression(
        std::unique_ptr<LogicalOrExpression> logical_or_expression,
        std::unique_ptr<Expression> then, std::unique_ptr<ConditionalExpression> else_p)
        : logical_or_expression_ptr(std::move(logical_or_expression)), then_ptr(std::move(then)),
          else_ptr(std::move(else_p))
    {
    }
    void ConditionalExpression::accept(IASTVisitor *visitor)
    {
        visitor->visit(this);
    }

    AssignmentExpression::AssignmentExpression(
        std::unique_ptr<ConditionalExpression> сonditional_expression,

        std::unique_ptr<UnaryExpression> unary_expression,
        std::unique_ptr<token> assignment_operator,
        std::unique_ptr<AssignmentExpression> assignment_expression)
        :

          сonditional_expression_ptr(std::move(сonditional_expression)),

          unary_expression_ptr(std::move(unary_expression)),
          assignment_operator_ptr(std::move(assignment_operator)),
          assignment_expression_ptr(std::move(assignment_expression))

    {
    }
    void AssignmentExpression::accept(IASTVisitor *visitor)
    {
        visitor->visit(this);
    }

    PostfixExpression::PostfixExpression(
        std::unique_ptr<PrimaryExpression> primary_expr,
        std::unique_ptr<Expression> expression_for_get_element_by_index,
        std::unique_ptr<ArgumentExpressionList> function_args_for_call,

        std::unique_ptr<token> prefix_operator, std::unique_ptr<token> identifier,
        std::unique_ptr<token> postfix_operator)
        :

          primary_expr_ptr(std::move(primary_expr)),
          expression_for_get_element_by_index_ptr(std::move(expression_for_get_element_by_index)),
          function_args_for_call_ptr(std::move(function_args_for_call)),

          prefix_operator_ptr(std::move(prefix_operator)), identifier_ptr(std::move(identifier)),
          postfix_operator_ptr(std::move(postfix_operator))
    {
    }
    void PostfixExpression::accept(IASTVisitor *visitor)
    {
        visitor->visit(this);
    }

    void ArgumentExpressionList::accept(IASTVisitor *visitor)
    {
        visitor->visit(this);
    }

    UnaryExpression::UnaryExpression(std::unique_ptr<token> prefix,
                                     std::unique_ptr<PostfixExpression> postfix,
                                     std::unique_ptr<UnaryOperator> unary_operator,
                                     std::unique_ptr<CastExpression> cast_expression,
                                     std::unique_ptr<TypeQualifier> type_qualifier)
        : prefix_operator(std::move(prefix)), postfix_ptr(std::move(postfix)),
          unary_operator_ptr(std::move(unary_operator)),
          cast_expression_ptr(std::move(cast_expression)),
          type_qualifier_ptr(std::move(type_qualifier))
    {
    }
    void UnaryExpression::accept(IASTVisitor *visitor)
    {
        visitor->visit(this);
    }

    UnaryOperator::UnaryOperator(token op) : value(op)
    {
    }
    void UnaryOperator::accept(IASTVisitor *visitor)
    {
        visitor->visit(this);
    }

    UsingDecl::UsingDecl(token token_of_identifier, std::unique_ptr<TypeQualifier> qualifier_ptr)
        : identifier(token_of_identifier), qualifier(std::move(qualifier_ptr))

    {
    }
    void UsingDecl::accept(IASTVisitor *visitor)
    {
        visitor->visit(this);
    }

    StorageModifier::StorageModifier(bool sm_is_static, bool sm_is_extern)
        : is_static(sm_is_static), is_extern(sm_is_extern)
    {
    }
    void StorageModifier::accept(IASTVisitor *visitor)
    {
        visitor->visit(this);
    }

    TypeQualifier::TypeQualifier(bool type_qf_is_const, bool type_qf_is_volatile)
        : is_const(type_qf_is_const), is_volatile(type_qf_is_volatile)
    {
    }
    void TypeQualifier::accept(IASTVisitor *visitor)
    {
        visitor->visit(this);
    }

    TypeName::TypeName(std::unique_ptr<TypeQualifier> qualifier_ptr, bool is_ptr,
                       std::unique_ptr<Type> type_ptr)
        : qualifier(std::move(qualifier_ptr)), ptr(is_ptr), type(std::move(type_ptr))

    {
    }
    void TypeName::accept(IASTVisitor *visitor)
    {
        visitor->visit(this);
    }

    Type::Type(token value_of_token) : value(value_of_token)
    {
    }
    void Type::accept(IASTVisitor *visitor)
    {
        visitor->visit(this);
    }
} // namespace ctc::ast
