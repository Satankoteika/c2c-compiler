#ifndef AST_VISITOR_H
#define AST_VISITOR_H

namespace ctc::ast
{
    class UsingDecl;
    class StorageModifier;
    class TypeQualifier;
    class TypeName;
    class Type;

    class IVisitor
    {
    public:
        virtual void visit(UsingDecl *element) = 0;
        virtual void visit(StorageModifier *element) = 0;
        virtual void visit(TypeQualifier *element) = 0;
        virtual void visit(TypeName *element) = 0;
        virtual void visit(Type *element) = 0;
    };
}

#endif
