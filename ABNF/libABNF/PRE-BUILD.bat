if not exist ..\..\Proto mkdir ..\..\Proto
if not exist ..\..\Proto\include mkdir ..\..\Proto\include
if not exist ..\..\Proto\include\SaS mkdir ..\..\Proto\include\SaS
if not exist ..\..\Proto\include\SaS\IETF mkdir ..\..\Proto\include\SaS\IETF
if not exist ..\..\Proto\include\SaS\IETF\ABNF mkdir ..\..\Proto\include\SaS\IETF\ABNF


xcopy ..\libABNF\*.hpp ..\..\Proto\include\SaS\IETF\ABNF /d /y

