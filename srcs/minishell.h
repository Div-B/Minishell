
#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include <string.h>
# include <fcntl.h>
# include <ctype.h>
# include <signal.h>
# include <errno.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>




// Define token type enumeration
enum	e_token
{
	WORD,
	SINGLE_Q,
	DOUBLE_Q,
	BUILTIN,
	PIPE,
	HEREDOC,
	IN_REDIR,
	OUT_REDIR,
	APPEND
};

// token structure
typedef struct s_token
{
	int				type;
	char			*string;
	int				is_redirection;
	int				merge_next;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

//heredoc list
typedef struct s_heredoc
{
	int					heredoc_flag;
	int					heredoc_count;
	char				*name;
	char				*delim;
	struct s_heredoc	*next;
	struct s_heredoc	*prev;
}	t_hdoc;

//command table
typedef struct s_table
{
	char				*cmd;
	char				**array;
	char				*infile;
	char				*outfile;
	char				*errfile;
	struct s_heredoc	*hdoc;
	int					heredoc_flag;
	int					append_flag;
	struct s_table		*next;
	struct s_table		*prev;
}	t_table;

// env list
typedef struct s_env
{
	char			*var;
	char			*value;
	char			**envp;
	struct s_env	*next;
}	t_env;

// file descriptors
typedef struct s_io_fds
{
	int	in_fd;
	int	pipe_fd[2];
}	t_io_fds;

extern int	g_exit_status;
//builtins.c
void	handle_pwd(void);
void	free_all(t_token **token_list, t_table **cmd_table, t_env **env_list);
void	exit_multiple_args(char **args, int *exit_num);
void	handle_exit(char **args, t_env **env_list, t_table **cmd_table,
			t_token **token_list);
void	handle_builtin(t_table *cmd_list,
			t_token *token_list, t_env **env_list);

//cd.c
void	change_directory(const char *path, t_env *env_list);
void	handle_cd(t_token *head, t_env *env_list);
void	update_env(t_env *env_list, const char *name, const char *value);

//cd_utils.c
void	update_pwd(t_env *env_list);

//command_table.c
t_token	*fill_array(int args_count, t_table *cmd_table, t_token *start);
t_token	*fill_cmd(t_table *cmd_table, t_token *current);
void	is_redir(t_token *current, t_table *cmd_table);
void	is_heredoc(t_token *current, t_table *cmd_table);
int		fill_table(t_token **token, t_table *cmd_table);

//command_table_utils.c
void	init_table(t_table *cmd_table);
int		count_args(t_token *current, t_table *cmd_table);
void	free_args(char **args);
void	free_cmd_table(t_table *cmd_table);
void	empty_cmd_str(t_table *cmd_table);

//hdoc.c
void	init_hdoc(t_table *cmd_table);
t_hdoc	*lst_last_hdoc(t_hdoc *hdoc);
t_hdoc	*create_next_hdoc(t_hdoc *hdoc);
char	*get_hdoc_name(t_table *cmd_table);
void	free_hdoc(t_hdoc *list);

//echo.c
t_table	*create_command_table(t_token **current);
void	handle_echo(t_table *cmd_table, t_env **env_list);

//env.c
void	exec_env(t_env *list);
t_env	*env_node(char *envp);
int		env_init(t_env **env_list, char **envp);
int		env_exists(t_env *list, char *s);
int		replace_env_str(t_env *list, char *str);

//env_utils.c
void	free_env_list(t_env *env_list);
int		valid_identifier(char *arg);
void	error_msg(char *arg, char *msg, int code);
int		check_unset(t_env *node, char *arg, t_env **list);
char	**env_list_to_envp(t_env *env_list);

//exec_utils.c
int		count_env_vars(t_env *env_list);
void	setup_io_redirections(t_table *cmd_table, t_io_fds *io_fds);
void	io_heredoc(t_table *c_tab, t_io_fds *io_fds, t_env **env_list);
void	exec_proc(t_table *c_tab, t_io_fds *io_fds, t_token *tok, t_env **env);
void	exec_child(t_table *c_tab, t_io_fds *io_fds, t_token *tok, t_env **env);

//expansion_utils.c
int		expansion_required(t_token *token);
int		is_var_char(char c);
char	*env_path_finder(const char *name, t_env *list);
char	*extract_var_name(char *string, int *index);
char	*expand_variable(char *name, t_env *env_list);

//export_unset.c
void	add_or_replace(t_env *list, char *str);
int		is_exit_status(char *arg);
void	exec_export(t_env *list, char **arg);
void	unset_node(t_env **list, t_env *node);
void	exec_unset(t_env *list, char **arg);

//heredoc.c
int		exec_heredoc(t_table *cmd_table, t_env **env_list);
int		init_heredoc(t_hdoc *hdoc, int *fd, int *delim_flag, char **line);
int		heredoc_handler(t_hdoc *hdoc, t_env **env_list);
int		line_expansion_required(char *string);
char	*heredoc_var_expansion(char *line, t_env **env_list);

//heredoc_utils.c
void	need_expansion(t_hdoc *hdoc, char *line, int fd, t_env **env_list);
int		open_heredocfd(t_hdoc *hdoc);
void	handle_heredoc_files(t_table *cmd_table);
void	setup_heredoc_sig(void);
int		heredoc_loop(t_hdoc *hdoc, int fd, t_env **env_list);

//main.c
int		setup(const char *cmd, t_env **env, t_token **tok, t_table **cmd_table);
void	exec_clean(t_table *cmd_table, t_token *token_list, t_env **env_list);
void	execute_command(const char *command, t_env **env_list);
int		main(int argc, char **argv, char **envp);

//main_utils.c
int		init_com(const char *cmd, t_env **e_list,
			t_token **t_list, t_table **c_tab);
int		init_check(const char *cmd, t_env **env,
			t_token **tok, t_table **cmd_tab);

//path.c
char	*ft_strtok(char *str, const char *delim);
char	*construct_command_path(const char *command_name, t_env *env_list);

//redirections.c
void	setup_pipe(t_io_fds *io_fds);
int		handle_redirections(t_table *cmd_table);

//set_exit.c
int		wait_for_children(t_table *cmd_table);
t_env *create_env_node(const char *prefix, char *exit_status_str, t_env **env_list);
char	*int_to_str(int value);
void	set_exit_status(t_env **env_list, int last_status);
char *join2(const char *a, const char *b);

//signals.c
void	signal_handler(int sig);
void	sig_parent(void);
void	sig_child(void);
void	restore_prompt(int sig);
void	heredoc_signal(int sig);
void	restore_prompt2(int sig);

//token_builtins.c
int		is_builtin(char *str);
int		assign_builtin(t_token **token);

//token_checks.c
int		quotation_check(const char *string);
int		empty_string(const char *str);
int		parser_tree(t_token **token);
void	del_empty_str(t_token **token);

//token_list_utils.c
void	free_token_list(t_token *head);
t_token	*lst_new(int type, char *str);
t_token	*lst_last(t_token *list);
void	token_addback(t_token **list, t_token *node);
int		add_token(t_token **token_list, int type, char *string);

//token_merge.c
void	merge_tokens(t_token **token);
void	set_merge_flag(t_token *token, const char *string, int start);
void	del_token_node(t_token *lst);

//token_splits.c
int		token_split(const char *string, int i);
int		get_token(const char *c, int i);
void	quote_split(t_token **token, const char *string, int *i, int *start);
void	redir_split(t_token **token, const char *string, int *i, int *start);

//tokenizer.c
int		assign_word(t_token **token, const char *string, int i, int *start);
int		assign_quote(t_token **token, const char *string, int *i, int *start);
int		assign_split(t_token **token, const char *string, int *i, int *start);
int		assign_token(t_token **token, const char *string);
int		tokenize(t_token **token, const char *string, t_env **env_list);

//var_expansions.c
int		assign_var(t_token **token, t_env *env_list);
char	*handle_remainder(char *string, int index, char *final);
char	*add_to_final(char *expansion, char *og);
char	*get_initial_str(char *string, int *i);
char	*expand_var(char *string, t_env *env_list);

#endif
