#ifndef AST_VISITOR_H
#define AST_VISITOR_H

namespace ctc::ast
{
    class PrimaryExpression;
    class Expression;
    class CastExpression;
    class MultiplicativeExpression;
    class AdditiveExpression;
    class ShiftExpression;
    class RelationalExpression;
    class EqualityExpression;
    class AndExpression;
    class ExclusiveOrExpression;
    class InclusiveOrExpression;
    class LogicalAndExpression;
    class LogicalOrExpression;
    class ConditionalExpression;
    class AssignmentExpression;
    class PostfixExpression;
    class ArgumentExpressionList;
    class UnaryExpression;
    class UnaryOperator;
    class UsingDecl;
    class StorageModifier;
    class TypeQualifier;
    class TypeName;
    class Type;

    class IASTVisitor
    {
      public:
        virtual void visit(PrimaryExpression *element) = 0;
        virtual void visit(Expression *element) = 0;
        virtual void visit(CastExpression *element) = 0;
        virtual void visit(MultiplicativeExpression *element) = 0;
        virtual void visit(AdditiveExpression *element) = 0;
        virtual void visit(ShiftExpression *element) = 0;
        virtual void visit(RelationalExpression *element) = 0;
        virtual void visit(EqualityExpression *element) = 0;
        virtual void visit(AndExpression *element) = 0;
        virtual void visit(ExclusiveOrExpression *element) = 0;
        virtual void visit(InclusiveOrExpression *element) = 0;
        virtual void visit(LogicalAndExpression *element) = 0;
        virtual void visit(LogicalOrExpression *element) = 0;
        virtual void visit(ConditionalExpression *element) = 0;
        virtual void visit(AssignmentExpression *element) = 0;
        virtual void visit(PostfixExpression *element) = 0;
        virtual void visit(ArgumentExpressionList *element) = 0;
        virtual void visit(UnaryExpression *element) = 0;
        virtual void visit(UnaryOperator *element) = 0;
        virtual void visit(UsingDecl *element) = 0;
        virtual void visit(StorageModifier *element) = 0;
        virtual void visit(TypeQualifier *element) = 0;
        virtual void visit(TypeName *element) = 0;
        virtual void visit(Type *element) = 0;
    };
} // namespace ctc::ast

#endif
