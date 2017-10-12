%              _       _   _
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% add external current to model

function addCurrent(self,compartment,ext_current)
	% does the compartment exist?
	assert(any(strcmp(compartment,properties(self))),'Unknown compartment')

	assert(length(ext_current) == self.t_end/self.dt,'external current is the wrong length')

	% add the current to the chosen compartment
	self.(compartment).ext_current;
end
