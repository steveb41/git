#include "cache.h"

int advice_push_nonfastforward = 1;
int advice_status_hints = 1;
int advice_commit_before_merge = 1;
int advice_resolve_conflict = 1;
int advice_implicit_identity = 1;
int advice_detached_head = 1;

static struct {
	const char *name;
	int *preference;
} advice_config[] = {
	{ "pushnonfastforward", &advice_push_nonfastforward },
	{ "statushints", &advice_status_hints },
	{ "commitbeforemerge", &advice_commit_before_merge },
	{ "resolveconflict", &advice_resolve_conflict },
	{ "implicitidentity", &advice_implicit_identity },
	{ "detachedhead", &advice_detached_head },
};

static void advise(const char *advice, ...)
{
	va_list params;

	va_start(params, advice);
	vreportf("hint: ", advice, params);
	va_end(params);
}

int git_default_advice_config(const char *var, const char *value)
{
	const char *k = skip_prefix(var, "advice.");
	int i;

	for (i = 0; i < ARRAY_SIZE(advice_config); i++) {
		if (strcmp(k, advice_config[i].name))
			continue;
		*advice_config[i].preference = git_config_bool(var, value);
		return 0;
	}

	return 0;
}

int error_resolve_conflict(const char *me)
{
	error("'%s' is not possible because you have unmerged files.", me);
	if (advice_resolve_conflict) {
		/*
		 * Message used both when 'git commit' fails and when
		 * other commands doing a merge do.
		 */
		advise("Please, fix them up in the work tree,");
		advise("and then use 'git add/rm <file>' as");
		advise("appropriate to mark resolution and make a commit,");
		advise("or use 'git commit -a'.");
	}
	return -1;
}

void NORETURN die_resolve_conflict(const char *me)
{
	error_resolve_conflict(me);
	die("Exiting because of an unresolved conflict.");
}
