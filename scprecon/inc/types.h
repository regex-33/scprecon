#ifndef TYPES_H
# define TYPES_H

typedef struct s_context
{
    char    **env;
    char    *last_pwd;
	int		unset_path;
	int		hidden_path;
    int     last_status;
	int		save_all;
}          t_context;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_HERE,
	REDIR_OUT,
	REDIR_APPEND
}		t_redir_type;

typedef struct s_redir
{
	char			*filename;
	char			*delimiter;
	t_redir_type	type;
	int				fd;
	int				bak_fd;
}			t_redir;

typedef struct s_cmd
{
	t_list	*cmd_args;
	t_list	*redir_list;
}	t_cmd;

typedef struct s_prexec
{
	char	*cmd_name;
	char	**args;
	int		err;
}		t_prexec;


typedef struct s_info
{
	t_list	*processes;
}	t_info;

typedef enum e_token_type
{
		tok_undefined,
		tok_l_par,
		tok_r_par,
		tok_literal,
		tok_pipe,
		tok_op_or,
		tok_op_and,
		tok_redir
}			t_token_type;

typedef struct s_token
{
		char			*value;
		size_t			len;
		t_token_type	type;
}			t_token;

typedef enum e_node_type
{
	nt_undefined,
	nt_subcmd,
	nt_subredir,
	nt_simplecmd,
	nt_pipe,
	nt_and_if,
	nt_or_if,
	nt_io_redir,
} 	t_node_type;

typedef struct s_btree
{
	struct s_btree	*left;
	struct s_btree	*right;
	void			*data;
	t_node_type		type;
}			t_btree;

#endif