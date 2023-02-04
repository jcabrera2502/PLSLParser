# Pascal-Like Simple Language Interpreter 

The following are the EBNF rules for the language used by this interpreter:

Prog ::= PROGRAM IDENT; DeclBlock ProgBody
DeclBlock ::= VAR {DeclStmt;}
DeclStmt ::= Ident {, Ident} : (Integer | Real | String)
ProgBody ::= BEGIN {Stmt;} END
Stmt ::= AssigStmt | IfStmt | WriteLnStmt | ForStmt
WriteLnStmt ::= WRITELN (ExprList)
IfStmt ::= IF ( LogicExpr ) THEN Stmt [ELSE Stmt]
ForStmt ::= FOR Var := ICONST (TO | DOWNTO) ICONST DO Stmt
AssignStmt ::= Var := Expr
ExprList ::= Expr {, Expr}
Expr ::= Term {(+|-) Term}
Term ::= SFactor {( * | / ) SFactor}
SFactor ::= [(+ | -)] Factor
LogicExpr ::= Expr (= | > | <) Expr
Var ::= IDENT
Factor ::= IDENT | ICONST | RCONST | SCONST | (Expr)
