// Copyright INRIA
if or(lstcat(list(1,2),list(4))<>list(1,2,4))  then pause,end
if or(lstcat(list(1,2,4),list())<>list(1,2,4))  then pause,end
if or(lstcat(list(1),list(2,4))<>list(1,2,4))  then pause,end
if or(lstcat(list(),list(1,2,4))<>list(1,2,4))  then pause,end

if or(lstcat(list(1,2),4)<>list(1,2,4))  then pause,end
if or(lstcat(1,list(2,4))<>list(1,2,4)) then pause,end

if or(lstcat(list(1),list(2),list(4))<>list(1,2,4))  then pause,end

if or(lstcat(list(1,2),list(),4)<>list(1,2,4))  then pause,end

if or(lstcat(list([1 2],['my string';'matrix']),list('foo',%s))<>list([1 2],..
  ['my string';'matrix'],'foo',%s)) then pause,end
