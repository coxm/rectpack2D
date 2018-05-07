#pragma once
#include "insert_and_split.h"

namespace rectpack2D {
	class default_empty_spaces;

	template <bool allow_flip, class empty_spaces_provider = default_empty_spaces>
	class root_node {
		empty_spaces_provider empty_spaces;

	public:
		using output_rect_type = std::conditional_t<allow_flip, rect_xywhf, rect_xywh>;

	private:
		rect_wh current_aabb;

		void expand_aabb_with(const output_rect_type& result) {
			current_aabb.w = std::max(current_aabb.w, result.x + result.w);
			current_aabb.h = std::max(current_aabb.h, result.y + result.h); 
		}

	public:
		root_node(const rect_wh& r) {
			reset(r);
		}

		void reset(const rect_wh& r) {
			current_aabb = {};

			empty_spaces.reset();
			empty_spaces.add(rect_xywh(0, 0, r.w, r.h));
		}

		template <class F>
		std::optional<output_rect_type> insert(const rect_wh image_rectangle, F report_candidate_empty_space) {
			for (int i = empty_spaces.get_count() - 1; i >= 0; --i) {
				const auto candidate_space = empty_spaces.get(i);

				report_candidate_empty_space(candidate_space);

				auto accept_result = [this, i, image_rectangle, candidate_space](
					const created_splits& splits,
					const bool flipping_necessary
				) -> std::optional<output_rect_type> {
					empty_spaces.remove(i);

					for (int s = 0; s < splits.count; ++s) {
						if (!empty_spaces.add(splits.spaces[s])) {
							return std::nullopt;
						}
					}

					if constexpr(allow_flip) {
						const auto result = output_rect_type(
							candidate_space.x,
							candidate_space.y,
							image_rectangle.w,
							image_rectangle.h,
							flipping_necessary
						);

						expand_aabb_with(result);
						return result;
					}
					else {
						(void)flipping_necessary;

						const auto result = output_rect_type(
							candidate_space.x,
							candidate_space.y,
							image_rectangle.w,
							image_rectangle.h
						);

						expand_aabb_with(result);
						return result;
					}
				};

				auto try_to_insert = [&](const rect_wh& img) {
					return rectpack2D::insert_and_split(img, candidate_space);
				};

				if constexpr(!allow_flip) {
					if (const auto normal = try_to_insert(image_rectangle)) {
						return accept_result(normal, false);
					}
				}
				else {
					const auto normal = try_to_insert(image_rectangle);
					const auto flipped = try_to_insert(rect_wh(image_rectangle).flip());

					/* 
						If both were successful, 
						prefer the one that generated less remainder spaces.
					*/

					if (normal && flipped) {
						/* 
							To prefer normal placements instead of flipped ones,
							better_than will return true only if the flipped one generated
						   	*strictly* less space remainders.
						*/

						if (flipped.better_than(normal)) {
							return accept_result(flipped, true);
						}

						return accept_result(normal, false);
					}

					if (normal) {
						return accept_result(normal, false);
					}

					if (flipped) {
						return accept_result(flipped, true);
					}
				}
			}

			return std::nullopt;
		}

		decltype(auto) insert(const rect_wh& image_rectangle) {
			return insert(image_rectangle, [](auto&){ });
		}

		auto get_rects_aabb() const {
			return current_aabb;
		}

		const auto& get_empty_spaces() const {
			return empty_spaces;
		}
	};
}
