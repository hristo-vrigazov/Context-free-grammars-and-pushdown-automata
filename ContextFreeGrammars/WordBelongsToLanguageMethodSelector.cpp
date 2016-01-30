#include "WordBelongsToLanguageMethodSelector.h"
#include "PushdownAutomaton.h"
#include "ContextFreeGrammar.h"

WordBelongsToLanguageMethodSelector::WordBelongsToLanguageMethodSelector(const ContextFreeGrammar & cfg, const string word)
	: 
	word(word),
	rules(cfg.getRules()),
	initialVariable(cfg.getInitialVariable())
{
}

WordBelongsToLanguageMethodSelector::~WordBelongsToLanguageMethodSelector()
{
}

const bool WordBelongsToLanguageMethodSelector::usingCYK() const
{
	ContextFreeGrammar cfg(this->rules, this->initialVariable);

	if (cfg.isChomskyNormal())
	{
		vector<Rule> rulesEnumerated;
		rulesEnumerated.assign(this->rules.begin(), this->rules.end());
		// CYK
		unsigned n = word.length();
		unsigned r = rulesEnumerated.size();
		map<pair<int, int>, set<Variable>> T;
		// initialize

		// get the terminal rules
		for (unsigned i = 0; i < n; i++)
		{
			for (unsigned j = 0; j < r; j++)
			{
				auto terminals = rulesEnumerated[j].getRightSide().getTerminals();
				if (terminals.size() > 0)
				{
					if (terminals.begin()->getLabel() == word[j])
					{
						T[pair<int, int>(i, 1)].insert(rulesEnumerated[j].getLeftSide());
					}
				}
			}
		}

		for (unsigned j = 2; j <= n; j++)
		{
			for (unsigned i = 0; i <= n - j; i++)
			{
				for (unsigned k = 1; k < j; k++)
				{
					for each (auto rule in cfg.getRules())
					{
						if (rule.getRightSide().length() == 2)
						{
							bool BisContained = T[pair<int, int>(i, k)].find(*rule.getRightSide().getVariables().begin()) != T[pair<int, int>(i, k)].end();
							bool CisContained = T[pair<int, int>(i + k, j - k)].find(*rule.getRightSide().getVariables().rbegin()) != T[pair<int, int>(i + k, j - k)].end();
						
							if (BisContained && CisContained)
							{
								T[pair<int, int>(i, j)].insert(rule.getLeftSide());
							}
						}
					}
				}
			}
		}


		return T[pair<int, int>(0, n)].find(initialVariable) != T[pair<int, int>(0, n)].end();
	}

	else
	{
		cfg.convertToChomskyNormal();
		WordBelongsToLanguageMethodSelector wblms(cfg, this->word);
		return wblms.usingCYK();
	}
}

const bool WordBelongsToLanguageMethodSelector::usingPushdownAutomaton() const
{
	PushdownAutomaton pd(this->rules, this->initialVariable);
	return pd.wordBelongsToLanguage(word);
}
