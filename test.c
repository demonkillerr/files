/* Create non synthetic file system - ext4*/
	if (stat(self->dir_path, &statbuf) != 0) 
	{
	    pid_t pid = fork();
		if (pid == -1) {
            perror("Failed to fork");
            exit(EXIT_FAILURE);
        } 
		else if (pid == 0) {
            char *fallocate_argv[] = {"fallocate", "--length", "4M", "test-ext4.img", NULL};
            execvp(fallocate_argv[0], fallocate_argv);
            perror("execvp failed");
            exit(EXIT_FAILURE);
		}
		else {
            int status;
            if (waitpid(pid, &status, 0) == -1) {
                perror("waitpid failed");
                exit(EXIT_FAILURE);
            }
            if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
                fprintf(stderr, "Failed to create ext4 filesystem image: fallocate failed\n");
                exit(EXIT_FAILURE);
            }
		}
	}

	/* Formate and mount non synthetic file system - ext4*/
	if (stat("mnt", &statbuf) != 0) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("Failed to fork");
            exit(EXIT_FAILURE);
        } 
		else if (pid == 0) {
            char *mkfs_argv[] = {"mkfs.ext4", "-q", "test-ext4.img", "mnt", NULL};
            execvp(mkfs_argv[0], mkfs_argv);
            perror("execvp failed");
            exit(EXIT_FAILURE);
        } 
		else {
            int status;
            if (waitpid(pid, &status, 0) == -1) {
                perror("waitpid failed");
                exit(EXIT_FAILURE);
            }
            if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
                fprintf(stderr, "Failed to format ext4 filesystem image: mkfs.ext4 failed\n");
                exit(EXIT_FAILURE);
            }
        }
	}

// Add more file systems
FIXTURE_VARIANT_ADD(layout3_fs, ext4) {
	.mnt = {
		.type = "ext4",
	},
	.file_path = TMP_DIR "/dir/file",
};
