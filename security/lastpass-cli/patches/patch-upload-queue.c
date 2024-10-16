$NetBSD: patch-upload-queue.c,v 1.2 2024/08/01 20:10:15 vins Exp $

SunOS compatibility.
Compatibility fix for time_t on 32-bit arches.

--- upload-queue.c.orig	2024-05-16 16:29:10.000000000 +0000
+++ upload-queue.c
@@ -86,7 +86,7 @@ static void upload_queue_write_entry(con
 
 	for (serial = 0; serial < ULONG_MAX; ++serial) {
 		free(name);
-		xasprintf(&name, "upload-queue/%lu%04lu", time(NULL), serial);
+		xasprintf(&name, "upload-queue/%llu%04lu", (long long)time(NULL), serial);
 		if (!config_exists(name))
 			break;
 	}
@@ -110,8 +110,12 @@ static void upload_queue_cleanup_failure
 
 	while ((entry = readdir(dir))) {
 		_cleanup_free_ char *fn = NULL;
-
+#ifdef __sun
+		stat(entry->d_name, &sbuf);
+		if (sbuf.st_mode != S_IFREG)
+#else
 		if (entry->d_type != DT_REG && entry->d_type != DT_UNKNOWN)
+#endif
 			continue;
 
 		for (p = entry->d_name; *p; ++p) {
@@ -172,11 +176,19 @@ static char *upload_queue_next_entry(uns
 	char *result, *p;
 	DIR *dir = opendir(base_path);
 	struct dirent *entry;
+#ifdef __sun
+	struct stat s;
+#endif
 
 	if (!dir)
 		return NULL;
 	while ((entry = readdir(dir))) {
+#ifdef __sun
+		stat(entry->d_name, &s);
+		if (s.st_mode != S_IFREG)
+#else
 		if (entry->d_type != DT_REG && entry->d_type != DT_UNKNOWN)
+#endif
 			continue;
 
 		for (p = entry->d_name; *p; ++p) {
