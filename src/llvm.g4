grammar Bkornblume;


program: (global_assignment | declaration | fuctions)*;
label: id ':';
id: ID;
value: INT | TRUE | FALSE;
type: TYPE;
global_assignment: id '=' 'global' type value;
fuctions: DEFINE (type | VOID) id '(' (type id)* ')' '{' (sentence)* '}';
declaration: DECLARE (type | VOID) id '(' (type id)* ')';
sentence:
	id '=' 'alloca' type						# alloca
	| id '=' 'load' type ',' 'ptr' id			# load
	| 'store' type (id | value) ',' 'ptr' id	# store
    | 'getelementptr' type ',' 'ptr' id(',' type id)* # getelementptr
    | id '=' 'icmp' op =(EQ | NE | UGT | UGE | ULT | ULE | SGT | SGE | SLT | SLE) type id ',' id # icmp
    | id '=' op =(ADD | SUB | MUL | SDIV | SREM | SHL | ASHR | AND | OR | XOR) type id ',' id # binary
    | id '=' 'call' type id '(' (type id)* ')' # call_with_return
    | 'call' VOID id '(' (type id)* ')' # call_without_return
    | id '=' 'phi' type ('['(value | id) ',' id']')* # phi
    | 'br' 'i1' id | value ',' 'label' id ',' 'label' id # conditional_br
    | 'br' 'label' id # unconditional_br
    | 'ret' type id # ret
    | 'ret' 'void' # ret_void
    | 'select' 'i1' (id | value) ',' type id ',' type id # select;

ID: ('%') [-a-zA-Z$._][-a-zA-Z$._0-9]*
	| ('@') [-a-zA-Z$._][-a-zA-Z$._0-9]*;
TRUE: 'true';
FALSE: 'false';
DEFINE: 'define';
DECLARE: 'declare';
VOID: 'void';
TYPE: 'i32' | 'i8' | 'i1' | ID | 'ptr';
INT: [0-9]+;
EQ: 'eq';
NE: 'ne';
UGT: 'ugt';
UGE: 'uge';
ULT: 'ult';
ULE: 'ule';
SGT: 'sgt';
SGE: 'sge';
SLT: 'slt';
SLE: 'sle';
ADD: 'add';
SUB: 'sub';
MUL: 'mul';
SDIV: 'sdiv';
SREM: 'srem';
SHL: 'shl';
ASHR: 'ashr';
AND: 'and';
OR: 'or';
XOR: 'xor';