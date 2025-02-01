#include <print_ast.h>
#include <ast.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ast.h>

void print_ast(Expr* expr)
{
  if (!expr) return;
  if (expr->tag == VARFREE)
  {
    printf("%s", expr->varfree.name);
    return;
  }
  if (expr->tag == VAR)
  {
    printf("$%u", expr->var.index);
    return;
  }
  if (expr->tag == TYPE)
  {
    printf("(Type %u)", expr->type.level);
    return;
  }
  if (expr->tag == ZERO)
  {
    printf("void");
    return;
  }
  if (expr->tag == ONE)
  {
    printf("one");
    return;
  }
  if (expr->tag == UNIT)
  {
    printf("unit");
    return;
  }
  if (expr->tag == TWO)
  {
    printf("bool");
    return;
  }
  if (expr->tag == FALSE_T)
  {
    printf("false");
    return;
  }
  if (expr->tag == TRUE_T)
  {
    printf("true");
    return;
  }
  if (expr->tag == APP)
  {
    printf("(");
    print_ast(expr->ind2.C);
    printf(" ");
    print_ast(expr->ind2.x);
    if (expr->ind2.y)
    {
      printf(" ");
      print_ast(expr->ind2.y);
    }
    if (expr->ind2.z)
    {
      printf(" ");
      print_ast(expr->ind2.z);
    }
    printf(")");
    return;
  }
  if (expr->tag == LAM)
  {
    /*
    printf("((λ. ");
    print_ast(expr->lam.expr);
    printf(") : ");
    print_ast(expr->lam.type_annot);
    printf(")");
    */
    printf("(λ. ");
    print_ast(expr->lam.expr);
    printf(")");
    return;
  }
  if (expr->tag == PI)
  {
    printf("(Π ");
    print_ast(expr->pi.dom);
    printf(", ");
    print_ast(expr->pi.cod);
    printf(")");
    return;
  }
  if (expr->tag == SIG)
  {
    printf("(Σ ");
    print_ast(expr->sig.f_ty);
    printf(", ");
    print_ast(expr->sig.family);
    printf(")");
    return;
  }
  if (expr->tag == PAIR)
  {
    printf("([");
    print_ast(expr->ind2.x);
    printf(", ");
    print_ast(expr->ind2.y);
    printf("] : ");
    print_ast(expr->ind2.C);
    printf(")");
    return;
  }
  if (expr->tag == FST)
  {
    printf("(fst ");
    print_ast(expr->ind2.C);

    printf(")");
    return;
  }
  if (expr->tag == SND)
  {
    printf("(snd ");
    print_ast(expr->ind2.C);

    printf(")");
    return;
  }
  if (expr->tag == ID)
  {
    printf("(eq ");
    print_ast(expr->ind2.C);
    printf(" ");
    print_ast(expr->ind2.x);
    printf(" ");
    print_ast(expr->ind2.y);

    printf(")");
    return;
  }
  if (expr->tag == REFL)
  {
    printf("(refl ");
    print_ast(expr->ind2.C);
    printf(" ");
    print_ast(expr->ind2.x);

    printf(")");
    return;
  }
  if (expr->tag == J)
  {
    printf("(J ");
    print_ast(expr->ind2.C);
    printf(" ");
    print_ast(expr->ind2.x);
    printf(" ");
    print_ast(expr->ind2.y);
    printf(" ");
    print_ast(expr->ind2.z);

    printf(")");
    return;
  }
  if (expr->tag == W)
  {
    printf("(W ");
    print_ast(expr->ind2.C);
    printf(" ");
    print_ast(expr->ind2.x);

    printf(")");
    return;
  }
  if (expr->tag == TREE)
  {
    printf("(tree ");
    print_ast(expr->ind2.C);
    printf(" ");
    print_ast(expr->ind2.x);

    printf(")");
    return;
  }
  if (expr->tag == REC)
  {
    printf("(rec ");
    print_ast(expr->ind2.C);
    printf(" ");
    print_ast(expr->ind2.x);
    printf(" ");
    print_ast(expr->ind2.y);

    printf(")");
    return;
  }
  if (expr->tag == IND0)
  {
    printf("(ind0 ");
    print_ast(expr->ind2.C);
    printf(" ");
    print_ast(expr->ind2.x);

    printf(")");
    return;
  }
  if (expr->tag == IND1)
  {
    printf("(ind1 ");
    print_ast(expr->ind2.C);
    printf(" ");
    print_ast(expr->ind2.x);
    printf(" ");
    print_ast(expr->ind2.y);
    printf(")");
    return;
  }
  if (expr->tag == IND2)
  {
    printf("(ind2 ");
    print_ast(expr->ind2.C);
    printf(" ");
    print_ast(expr->ind2.x);
    printf(" ");
    print_ast(expr->ind2.y);
    printf(" ");
    print_ast(expr->ind2.z);

    printf(")");
    return;
  }
  printf("(unexpected tag: %d)", expr->tag);
  exit(0);
}
