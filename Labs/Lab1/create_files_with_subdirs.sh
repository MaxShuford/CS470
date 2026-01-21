# Env var for date and time
MAIN_DIR=$(date +"%Y-%m-%d_%H-%M-%S")
# Env for log file: 
LOG_FILE="script.log"

languages=(
  Python
  Java
  C
  "C++"
  JavaScript
  Ruby
  Go
  Rust
  Swift
  Kotlin
)

# Log function to:
# 1. accepts any text
# 2. pipes to file
# 3. set to append "LOG FILE"

log() {
  echo "$(date '+%Y-%m-%d %H:%M:%S') - $*" | tee -a "$LOG_FILE"
}

# create
mkdir -p "$MAIN_DIR"
log "Created main directory: $MAIN_DIR"

# Error handle
if [[ -d "$MAIN_DIR" ]]; then
  log "VERIFY OK: Main directory exists."
else
  log "VERIFY FAIL: Main directory was not created."
  exit 1
fi

# Create log file in parent
: > "$LOG_FILE"
log "Script started."


# Sub Dirs,
for i in {101..110}; do
  SUBDIR="$MAIN_DIR/file$i"
  mkdir -p "$SUBDIR"
  log "Created subdirectory: $SUBDIR"
done

for i in {101..110}; do
  SUBDIR="$MAIN_DIR/file$i"

  idx=0
  for j in {501..510}; do
    FILE="$SUBDIR/tuser$j.txt"
    echo "${languages[$idx]}" > "$FILE"
    log "Created file: $FILE (content: ${languages[$idx]})"
    idx=$((idx + 1))
  done
done

log "All tasks completed successfully."
log "Script finished."
