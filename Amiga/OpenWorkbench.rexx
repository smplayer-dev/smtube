/* REXX */
parse arg PATH
ADDRESS WORKBENCH

PATH = right(PATH,length(PATH)-1)
A = pos("/",PATH)
if A > 0,
then do
  /*say A*/
  PATH = OVERLAY(':',PATH,A)
end

PATH = INSERT('"',PATH)
PATH = INSERT('"',PATH,length(PATH))
/*say PATH*/

WINDOW PATH OPEN