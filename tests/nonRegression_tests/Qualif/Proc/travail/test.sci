disp " C EST MON TEST"
disp envq_ver

if ~isdir (envq_log+envq_ver) then
unix("mkdir "+envq_log+envq_ver)
else
disp "la directory existe"
end