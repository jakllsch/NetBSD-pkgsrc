$NetBSD: patch-Makefile.PL,v 1.4 2020/01/01 05:14:22 mef Exp $

We are not MSWin32, remove this line to avoid import another package.

--- Makefile.PL.orig	2019-12-07 10:25:54.000000000 +0900
+++ Makefile.PL	2020-01-01 14:12:14.162520580 +0900
@@ -1,4 +1,3 @@
-use Devel::AssertOS qw[-MSWin32];
 # This Makefile.PL for MooseX-Daemonize was generated by
 # Dist::Zilla::Plugin::MakeMaker::Fallback 0.030
 # and Dist::Zilla::Plugin::MakeMaker::Awesome 0.48.
