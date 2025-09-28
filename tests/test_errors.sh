# we'll cd to here
ROOT_FOLDER="../"
EXECUTABLE="cub3D"

VAL_OUT_PREFIX="valgrind"

EXPECTED_ERROR_MSG="Error"

# Test counters
TESTS_PASSED=0
TESTS_FAILED=0

# mapas para testes
MAPS_DIR="assets/maps/"
SHOULD_PASS_DIR="Correct/"
SHOULD_FAIL_DIR="Wrong/"

RED="\033[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[0;33m"
BLUE="\033[0;34m"
MAGENTA="\033[0;35m"
CYAN="\033[0;36m"
WHITE="\033[0;37m"
RESET="\033[0m"

test_should_pass() {
	# make path to test map
	local map_to_test=$1
	local map_path="${MAPS_DIR}${SHOULD_PASS_DIR}${map_to_test}"

	# 1st run -> catch stderr
	local found_error=$(./"${EXECUTABLE}" "$map_path" 2>&1 >/dev/null)

	# 2nd run -> catch exit code
	./"${EXECUTABLE}" "$map_path" >/dev/null 2>/dev/null
	local exit_code=$?

	# 3rd run -> valgrind
	local val_out_file="${VAL_OUT_PREFIX}_${map_to_test%%.*}.out"
	valgrind --leak-check=full --error-exitcode=1 --log-file="$val_out_file" ./"$EXECUTABLE" "$map_path"
	local bytes_not_freed=$(grep "in use at exit:" "$val_out_file" | sed 's/.*in use at exit: \([0-9,]*\) bytes.*/\1/' | tr -d ',')
	bytes_not_freed=${bytes_not_freed:-0}  # default to 0 if empty

	if [ -n "$found_error" ] || [ $exit_code -ne 0 ] || [ $bytes_not_freed -gt 0 ]; then
		# something is not ok
		# so print invidually
		# for better diagnostics
		if [ -n "$found_error" ];  then
			printf "${RED}[KO]${RESET}"
		else
			printf "${GREEN}[OK]${RESET}"
		fi
		if [ $exit_code -eq 1 ] ;  then
			printf "${RED}[KO]${RESET}"
		else
			printf "${GREEN}[OK]${RESET}"
		fi
		if [ $bytes_not_freed -gt 0 ];  then
			printf "${RED}[KO]${RESET}"
		else
			printf "${GREEN}[OK]${RESET}"
		fi

		echo ""
		printf "Failed while parsing map: $map_to_test\n"
		printf "What went wrong?\n"
		if [ -n "$found_error" ]; then
			printf "${EXECUTABLE} said: \"$found_error\"\n"
		fi
		if [ $exit_code -eq 1 ]; then
			printf "exit code: $exit_code\n"
		fi
		if [ $bytes_not_freed -gt 0 ]; then
			printf "leaks: go to $val_out_file\n"
		fi
		TESTS_FAILED=$((TESTS_FAILED + 1))
	else
		printf "${GREEN}[OK][OK][OK]${RESET}\t$map_to_test\n"
		rm $val_out_file
		TESTS_PASSED=$((TESTS_PASSED + 1))
	fi
}

test_should_fail() {
	# $1: ficheiro para dar ao executável
	local map_to_test=$1

	local map_path="${MAPS_DIR}${SHOULD_FAIL_DIR}${map_to_test}"

	#1st run -> catch stderr
	local found_error=$(./"${EXECUTABLE}" "$map_path" 2>&1 >/dev/null)

	#2nd run -> catch exit code
	./"${EXECUTABLE}" "$map_path" >/dev/null 2>/dev/null
	local exit_code=$?

	# 3rd run -> valgrind
	local val_out_file="${VAL_OUT_PREFIX}_${map_to_test%%.*}.out"
	valgrind --leak-check=full --error-exitcode=1 --log-file="$val_out_file" ./"$EXECUTABLE" "$map_path"
	local bytes_not_freed=$(grep "in use at exit:" "$val_out_file" | sed 's/.*in use at exit: \([0-9,]*\) bytes.*/\1/' | tr -d ',')
	bytes_not_freed=${bytes_not_freed:-0}  # default to 0 if empty

	if [ -n "$found_error" ] && [ $exit_code -eq 1 ] && [ $bytes_not_freed -eq 0 ]; then
		printf "${GREEN}[OK][OK][OK]${RESET}\t$map_to_test\n"
		rm $val_out_file
		TESTS_PASSED=$((TESTS_PASSED + 1))
	else
		# something is not ok
		# so print invidually
		# for better diagnostics
		if [ -z "$found_error" ];  then
			printf "${RED}[KO]${RESET}"
		else
			printf "${GREEN}[OK]${RESET}"
		fi
		if [ "$exit_code" -ne 1 ];  then
			printf "${RED}[KO]${RESET}"
		else
			printf "${GREEN}[OK]${RESET}"
		fi
		if [ $bytes_not_freed -gt 0 ]; then
			printf "${RED}[KO]${RESET}"
		else
			printf "${GREEN}[OK]${RESET}"
		fi

		echo ""
		printf "Failed while parsing map: $map_to_test\n"
		printf "What went wrong?:\n"
		if [ -z "$found_error" ] ;  then
			printf "${EXECUTABLE} not saying anything\n"
		fi
		if [ $exit_code -ne 1 ]; then
			printf "exit code: $exit_code\n"
		fi
		if [ $bytes_not_freed -gt 0 ]; then
			printf "leaks: go to $val_out_file\n"
		fi
		TESTS_FAILED=$((TESTS_FAILED + 1))
	fi
}

cd $ROOT_FOLDER 
echo "Reminder: set console logs to nothing"
echo "Testing: [Stderror Output][Exit Code][Leaks]"
echo "==========="
echo "SHOULD PASS"
echo "==========="
printf 'Expecting: Nothing in std error; Exit code always 0; No leaks\n'
test_should_pass "map_longest_rows.cub"
test_should_pass "map_max_rows.cub"
test_should_pass "map_multiple_spaces_in_each_line.cub"
test_should_pass "map_non_rectangular1.cub"
test_should_pass "map_non_rectangular2.cub"
test_should_pass "map_one_space_in_each_line.cub"
test_should_pass "map_only_walls.cub"
test_should_pass "map_smallest.cub"
test_should_pass "mixed_colors_first.cub"
test_should_pass "mixed_colors_in_between_textures.cub"
test_should_pass "textures_reverse_order.cub"
test_should_pass "empty_spaces_at_bottom.cub"
test_should_pass "empty_spaces_at_top.cub"
test_should_pass "empty_spaces_inside_at_left.cub"
test_should_pass "empty_spaces_inside_not_out.cub"
test_should_pass "subject_pdf_example.cub"
test_should_pass "walls_outside.cub"

echo ""
echo "==========="
echo "SHOULD FAIL"
echo "==========="
printf 'Expecting: \"Error\\n\" in std error; Exit code always 1; No leaks\n'
test_should_fail "colors_1_string.cub"
test_should_fail "colors_above_255.cub"
test_should_fail "colors_below_0.cub"
test_should_fail "colors_equal_ids.cub"
test_should_fail "colors_less_than_3_values.cub"
test_should_fail "colors_more_than_2_strings.cub"
test_should_fail "colors_more_than_3_values.cub"
test_should_fail "colors_multiple_comas.cub"
test_should_fail "colors_overflow.cub"
test_should_fail "colors_wrong_id.cub"
test_should_fail "map_at_beginning_of_file.cub"
test_should_fail "map_at_middle_of_file.cub"
test_should_fail "map_divided_in_multiple_parts_throughout_file.cub"
test_should_fail "map_longest_row_is_open_spaces.cub"
test_should_fail "map_negative_nums.cub"
test_should_fail "map_no_walls.cub"
test_should_fail "map_non_binary_nums.cub"
test_should_fail "map_open_bottom_left_corner.cub"
test_should_fail "map_open_bottom_right_corner.cub"
test_should_fail "map_open_middle_bottom.cub"
test_should_fail "map_open_middle_left.cub"
test_should_fail "map_open_middle_right.cub"
test_should_fail "map_open_middle_top.cub"
test_should_fail "map_open_top_left_corner.cub"
test_should_fail "map_open_top_right_corner.cub"
test_should_fail "map_player_no_position.cub"
test_should_fail "map_player_orientation_invalid_character.cub"
test_should_fail "map_player_position_case_sensitivity.cub"
test_should_fail "map_row_too_long.cub"
test_should_fail "map_tabs_in_each_line.cub"
test_should_fail "map_too_many_rows.cub"
test_should_fail "map_with_empty_lines.cub"
test_should_fail "map_with_multiple_player_positions.cub"
test_should_fail "map_with_multiple_player_positions_in_same_line.cub"
test_should_fail "textures_error_1_string.cub"
test_should_fail "textures_error_2_equal_ids.cub"
test_should_fail "textures_error_more_than_2_strings.cub"
test_should_fail "textures_error_wrong_extension.cub"
test_should_fail "textures_error_wrong_id.cub"
test_should_fail "textures_error_wrong_path.cub"
test_should_fail "empty_spaces_outside.cub"
test_should_fail "subject_pdf_example.xpm"

echo ""
echo "==========="
echo "TEST SUMMARY"
echo "==========="
TOTAL_TESTS=$((TESTS_PASSED + TESTS_FAILED))
printf "Total tests run: ${BLUE}$TOTAL_TESTS${RESET}\n"
printf "Tests passed: ${GREEN}$TESTS_PASSED${RESET}\n"
printf "Tests failed: ${RED}$TESTS_FAILED${RESET}\n"

if [ $TESTS_FAILED -eq 0 ]; then
	printf "\n${GREEN}🎉 ALL TESTS PASSED! 🎉${RESET}\n"
	exit 0
else
	printf "\n${RED}❌ $TESTS_FAILED TEST(S) FAILED${RESET}\n"
	exit 1
fi
