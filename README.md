# Pascal-Like Simple Language Interpreter 

The following are the EBNF rules for the language used by this interpreter:

1. Prog ::= PROGRAM IDENT; DeclBlock ProgBody
2. DeclBlock ::= VAR {DeclStmt;}
3. DeclStmt ::= Ident {, Ident} : (Integer | Real | String)
4. ProgBody ::= BEGIN {Stmt;} END
5. Stmt ::= AssigStmt | IfStmt | WriteLnStmt | ForStmt
6. WriteLnStmt ::= WRITELN (ExprList)
7. IfStmt ::= IF ( LogicExpr ) THEN Stmt [ELSE Stmt]
8. ForStmt ::= FOR Var := ICONST (TO | DOWNTO) ICONST DO Stmt
9. AssignStmt ::= Var := Expr
10. ExprList ::= Expr {, Expr}
11. Expr ::= Term {(+|-) Term}
12. Term ::= SFactor {( * | / ) SFactor}
13. SFactor ::= [(+ | -)] Factor
14. LogicExpr ::= Expr (= | > | <) Expr
15. Var ::= IDENT
16. Factor ::= IDENT | ICONST | RCONST | SCONST | (Expr)
